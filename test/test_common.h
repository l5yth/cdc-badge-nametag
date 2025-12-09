#ifndef TEST_COMMON_H
#define TEST_COMMON_H
#include <unity.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

void setup_usb_serial() {
    Serial.begin(115200);
    unsigned long start = millis();
    while (!Serial && (millis() - start) < 3000) {
        delay(10);
    }
    delay(500);
    Serial.println("\n--- Serial available ---");
}

#endif
