/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
  Funktion: ESP32 TFT 1.77" RTC
            Datum, Uhrzeit, Temperatur

**************************************************************************************************
  Version: 18.02.2023
**************************************************************************************************
  Board: ESP32vn IoT UNO
**************************************************************************************************
  Libraries:
  https://github.com/espressif/arduino-esp32/tree/master/libraries
  C:\Users\User\Documents\Arduino
  D:\gittemp\Arduino II\A156_Wetterdaten_V3
**************************************************************************************************
  C++ Arduino IDE V1.8.19
**************************************************************************************************
  Einstellungen:
  https://dl.espressif.com/dl/package_esp32_index.json
  http://dan.drown.org/stm32duino/package_STM32duino_index.json
  http://arduino.esp8266.com/stable/package_esp8266com_index.json

Verdrahtung:
ESP32 1.77" TFT Display
GND   GND
5V    VCC
18    SCK
23    SDA (MOSI)
14    RES (RST)
13    RS (DC)
12    CS
3.3V  LEDA


RTC -> G, V, SCL, SDA

------------------------------------------------------------------------------------------------
// TFT 1.77" Library

#define ST7735_DRIVER
#define TFT_WIDTH 128
#define TFT_HEIGHT 160
#define ST7735_REDTAB
#define TFT_BACKLIGHT_ON HIGH // HIGH or LOW are options

// ###### EDIT THE PIN NUMBERS IN THE LINES FOLLOWING TO SUIT YOUR ESP32 SETUP ######
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 12 // Chip select control pin
#define TFT_DC 13 // Data Command control pin
#define TFT_RST 14 // Reset pin (could connect to RST pin)

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
#define SMOOTH_FONT
#define SPI_FREQUENCY  27000000 // Actually sets it to 26.67MHz = 80/3

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000

// Transactions are automatically enabled by the library for an ESP32 (to use HAL mutex)
// so changing it here has no effect

// #define SUPPORT_TRANSACTIONS  
**************************************************************************************************/

#include <Arduino.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {
  "Sonntag",
  "Montag",
  "Dienstag",
  "Mittwoch",
  "Donnerstag",
  "Freitag",
  "Samstag"
};




// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 2000

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0; // Used for testing draw times

void setup(void) {
  Serial.begin(115200);

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  /* To manualy set date and time,
    remove the coment // signs
    and enter new values in the followingline
    in this sequence: year, day, month, hour, minute and second.*/
  //rtc.adjust(DateTime(2020, 2, 24, 10, 00, 0));



  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //weiss
}

void loop() {
  DateTime now = rtc.now();

  //********* TFT *****************
  tft.setTextSize (3);

  //---------- Seite 1 ----------------------------



  tft.drawString((daysOfTheWeek[now.dayOfTheWeek()]), 15, 2);



  //Datum
  tft.setTextSize (2);

  if (now.day() < 10) {
    tft.print("0");
    tft.setCursor (18, 30);
    tft.print(now.day());
  }
  else {
    tft.setCursor (18, 30);
    tft.print(now.day(), DEC);
  }
  tft.print(".");
  if (now.month() < 10) {
    tft.print("0");
    tft.print(now.month());
  }
  else {
    tft.print(now.month(), DEC);
  }
  tft.print(".");
  tft.print(now.year(), DEC);



  //Uhrzeit
  tft.setTextColor(0xFBE0, TFT_BLACK);
  tft.setTextSize (3);
  tft.setCursor(5, 60);
  if (now.hour() < 10) {
    tft.print("0");
    tft.print(now.hour());
  }
  else {
    tft.print(now.hour(), DEC);
  }
  tft.print(':');
  if (now.minute() < 10) {
    tft.print("0");
    tft.print(now.minute());
  }
  else {
    tft.print(now.minute(), DEC);
  }
  tft.print(':');
  if (now.second() < 10) {
    tft.print("0");
    tft.print(now.second());
  }
  else {
    tft.print(now.second(), DEC);
  }

  //Temperatur
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize (2);
  tft.setCursor(40, 95);
  tft.print(rtc.getTemperature(), (1));
  tft.print("\xF7""C");
}
