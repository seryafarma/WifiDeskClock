#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <ezTime.h>

#ifndef STASSID
#define STASSID "Insert your SSID here."
#define STAPSK "Insert your pass here."
#endif

Timezone amst;

// Turn on debug statements to the serial output
#define DEBUG 1

#if DEBUG
#define PRINT(s, x)         \
    {                       \
        Serial.print(F(s)); \
        Serial.print(x);    \
    }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)

#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)

#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

// For ESP8266:
#define DATA_PIN 7  // or MOSI
#define CS_PIN 4    // or SS
#define CLK_PIN 5   // or SCK

// For normal UNO:
//#define DATA_PIN  11  // or MOSI
//#define CS_PIN    10  // or SS
//#define CLK_PIN   13  // or SCK

// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// We always wait a bit between updates of the display
#define DELAYTIME 100   // in milliseconds
#define SLEEPTIME 2000  // in milliseconds
#define TIMEZONE "Europe/Amsterdam"

void rows() {
    PRINTS("\nRows 0->7");
    mx.clear();

    for (uint8_t row = 0; row < ROW_SIZE; row++) {
        mx.setRow(row, 0xff);
        delay(2 * DELAYTIME);
        mx.setRow(row, 0x00);
    }
}

void printTime(String* time) {
    char clockchar[5];
    time->toCharArray(clockchar, 5);

    mx.clear();
    mx.update(MD_MAX72XX::OFF);

    mx.setRow(2, 2, 0x01);
    mx.setRow(2, 4, 0x01);
    mx.setChar(1 * COL_SIZE - 2, clockchar[3]);
    mx.setChar(2 * COL_SIZE - 2, clockchar[2]);
    mx.setChar(3 * COL_SIZE - 1, clockchar[1]);
    mx.setChar(4 * COL_SIZE - 1, clockchar[0]);

    mx.update();
    mx.update(MD_MAX72XX::ON);
}

void setup() {
    mx.begin();

#if DEBUG
    Serial.begin(9600);
#endif
    WiFi.begin(STASSID, STAPSK);
    waitForSync();

#if DEBUG
    Serial.println("UTC: " + UTC.dateTime());
#endif
    amst.setLocation(TIMEZONE);
}

void loop() {
    static unsigned scanline = 0;
#if DEBUG
    Serial.println("Amsterdam time: " + amst.dateTime("H:i:s"));
#endif
    String hm = amst.dateTime("Hi");

    // Push the time.
    printTime(&hm);
    // Just sleep here before doing anything else.
    delay(DELAYTIME);
    scanline++;

    // Simple animation for this clock, after around 30 seconds do a scanline.
    if (scanline == 15) {
        scanline = 0;
        rows();
    }
}

