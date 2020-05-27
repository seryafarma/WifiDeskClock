#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <ezTime.h>

#ifndef STASSID
#define STASSID "Insert yuor SSID here."
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
#define DELAYTIME 100  // in milliseconds

void scrollText(const char* p) {
    uint8_t charWidth;
    uint8_t cBuf[8];  // this should be ok for all built-in fonts

    PRINTS("\nScrolling text");
    mx.clear();

    while (*p != '\0') {
	charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);

	for (uint8_t i = 0; i <= charWidth;
	     i++)  // allow space between characters
	{
	    mx.transform(MD_MAX72XX::TSL);
	    if (i < charWidth) mx.setColumn(0, cBuf[i]);
	    delay(DELAYTIME);
	}
    }
}

void rows()
// Demonstrates the use of setRow()
{
    PRINTS("\nRows 0->7");
    mx.clear();

    for (uint8_t row = 0; row < ROW_SIZE; row++) {
	mx.setRow(row, 0xff);
	delay(2 * DELAYTIME);
	mx.setRow(row, 0x00);
    }
}

void printTime(String* time)
// Run through display of the the entire font characters set
{
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
    Serial.begin(57600);
#endif
    PRINTS("\n[MD_MAX72XX Test & Demo]");
    WiFi.begin(STASSID, STAPSK);

    waitForSync();

    Serial.println("UTC: " + UTC.dateTime());

    amst.setLocation("Europe/Amsterdam");
}

void loop() {
    static unsigned scanline = 0;
    Serial.println("Amsterdam time: " + amst.dateTime("H:i:s"));
    String hm = amst.dateTime("Hi");

    printTime(&hm);
    delay(2000);
    scanline++;

    if (scanline == 15) {
	scanline = 0;
	rows();
    }
}

