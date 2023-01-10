// main.cpp
// Author  : yujinh@126.com
// Address : CUG @ Wuhan
// Date    : 2022-12-26
#include <Arduino.h>

#include "ndtCS1237.hpp"
#include "ndtStimuAB.hpp"

HardwareSerial &pc = Serial;

ndtCS1237 a1237(14, 12);
ndtCS1237 u1237(13, 15);

StimuAB ab(16, 17);

void setup() {
  u1237.powerdown();
  a1237.powerdown();

  pc.begin(115200);

  delay(1);  // cs1237 will power down in 100us

  u1237.poweron();
  a1237.poweron();
  delay(1);

  a1237.writeConfig(0, 0, 0, 0);
  u1237.writeConfig(0, 0, 0, 0);
  ab.b2a();
}

int tNext = 100;
void loopAB(int i);

int rs = 10;
void loop() {
  ab.b2a();
  delay(300);

  pc.println("b->a");

  for (int i = 0; i < rs; i++) {
    loopAB(i);
  }

  ab.a2b();
  delay(300);
  pc.println("a->b");
  for (int i = 0; i < rs; i++) {
    loopAB(i);
  }
}

void loopAB(int i) {
  delay(120);

  int u = u1237.read();
  int a = a1237.read();

  pc.printf("@%d,U= %08x,", millis(), u);
  pc.print(u, BIN);
  pc.printf(",%d,%f ,", u / 256, u / 256 * 0.5 * 3.3 / 0x7FFFFF);

  pc.printf(",I=%08x ,", a);

  pc.print(a, BIN);
  pc.printf(",%d", a / 256);
  pc.printf(",%f", a / 256 * 0.5 * 3.3 / 0x7FFFFF);
}
