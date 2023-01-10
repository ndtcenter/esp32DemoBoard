// main.cpp
// Author  : yujinh@126.com
// Address : CUG @ Wuhan
// Date    : 2022-12-26
#include <Arduino.h>
#include "ndtCS1237.hpp"
#include "ndtStimuAB.hpp"

#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:

// const char *networkName = "your WiFi's AP";
// const char *networkPswd = "your WiFi's password";
// const char *udpAddress = "IP of your test pc";

// const char *networkName = "ndth";
// const char *networkPswd = "hjwx6hjwx";
// const char *udpAddress = "192.168.31.176";

const char *networkName = "ndto";
const char *networkPswd = "18971675775*hyj";
const char *udpAddress = "192.168.31.176";

const int udpPort = 3333;
HardwareSerial &pc = Serial;

int rgb[] = {0, 2, 4};
int rgb2[] = {25, 32, 33, 12};

ndtCS1237 a1237(14, 12);
ndtCS1237 u1237(13, 15);

StimuAB ab(16, 17);

// Are we currently connected?
boolean connected = false;

// The udp library class
WiFiUDP udp;

// wifi event handler
void WiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        // When connected set
        Serial.print("WiFi connected! IP address: ");
        Serial.println(WiFi.localIP());
        // initializes the UDP state
        // This initializes the transfer buffer
        udp.begin(WiFi.localIP(), udpPort);
        connected = true;
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        connected = false;
        break;
    default:
        break;
    }
}

void connectToWiFi(const char *ssid, const char *pwd)
{
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);
    // register event handler
    WiFi.onEvent(WiFiEvent);

    // Initiate connection
    WiFi.begin(ssid, pwd);

    Serial.println("Waiting for WIFI connection...");
}

void setup()
{
    for (int i = 0; i < 3; i++)
    {
        pinMode(rgb[i], OUTPUT);
        pinMode(rgb2[i], OUTPUT);
        digitalWrite(rgb[i], LOW);
        digitalWrite(rgb2[i], LOW);
    }

    u1237.powerdown();
    a1237.powerdown();
    pc.begin(115200);

    // Connect to the WiFi network
    connectToWiFi(networkName, networkPswd);

    // ab.a2b();

    ab.b2a();

    delay(1);
    u1237.poweron();
    a1237.poweron();
    a1237.writeConfig(0, 0, 0, 0);
    u1237.writeConfig(0, 0, 0, 0);
}

int tNext = 100;
void loopAB(int i);

int rs = 10;
void loop()
{
    ab.b2a();
    delay(300);
    udp.beginPacket(udpAddress, udpPort);
    udp.printf("b-》a");
    udp.endPacket();

    for (int i = 0; i < rs; i++)
    {
        loopAB(i);
    }

    ab.a2b();
    delay(300);
    udp.beginPacket(udpAddress, udpPort);
    udp.printf("a-》b");
    udp.endPacket();
    for (int i = 0; i < rs; i++)
    {
        loopAB(i);
    }
}

void loopAB(int i)
{
    delay(120);

    int u = u1237.read();
    int a = a1237.read();

    if (connected)
    {
        // bt.print(millis());
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("@%d,U= %08x,", millis(), u);
        udp.print(u, BIN);
        udp.printf(",%d,%f ,", u / 256, u / 256 * 0.5 * 3.3 / 0x7FFFFF);

        udp.printf(",I=%08x ,", a);

        udp.print(a, BIN);
        udp.printf(",%d", a / 256);
        udp.printf(",%f", a / 256 * 0.5 * 3.3 / 0x7FFFFF);
        udp.endPacket();
    }
}
