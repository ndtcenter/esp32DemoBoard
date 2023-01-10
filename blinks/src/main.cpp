#include <Arduino.h>

int rgb[] = {0, 2, 4};
int rgb2[] = {25, 32, 33};

void digitalToggle(int pin) { digitalWrite(pin, !digitalRead(pin)); }

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(rgb[i], OUTPUT);
    pinMode(rgb2[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalToggle(rgb[i]);
    digitalToggle(rgb2[i]);
    delay(300);
    digitalToggle(rgb[i]);
    digitalToggle(rgb2[i]);
    delay(300);
  }
}