#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Adafruit_TinyUSB.h>
#include "key_config.h"
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
const int scanPin = 45;
int backlight_value = 125;
void setup() {
  Wire.begin(42, 41);
  Serial.begin(115200);
  pinMode(scanPin, OUTPUT);
  digitalWrite(scanPin, LOW);  // Start with pin 45 low
  delay(500);                  // Wait for devices to settle (adjust the delay as needed)
  pinMode(21, OUTPUT);         //i2c power
  digitalWrite(21, HIGH);
  pinMode(TFT_BACKLITE, ANALOG);
  analogWrite(TFT_BACKLITE, 124);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
}

#define COMPASS_DEVICEADDRESS 0x12
void readBytes(uint8_t addr, uint8_t *data, uint8_t size) {
Wire.beginTransmission(COMPASS_DEVICEADDRESS);

  Wire.write(addr);
    uint8_t err = Wire.endTransmission(false);
    if(err!= ESP_OK){
      Serial.println(err);
    }
  Wire.requestFrom((uint16_t)COMPASS_DEVICEADDRESS, size);
  for (uint8_t i = 0; i < size; ++i) {
    data[i] = Wire.read();
  }

}

uint8_t dump, lastdump;
void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for (t = 0; t <= 4; t += 1) {
    int x = 0;
    int y = 0;
    int w = tft.width() - 2;
    int h = tft.height() - 2;
    for (i = 0; i <= 16; i += 1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x += 2;
      y += 3;
      w -= 4;
      h -= 6;
      color += 1100;
    }
    color += 100;
  }
}
void loop() {
  byte error, address;
  int deviceCount = 0;
  uint8_t buffer_size = 0;
  readBytes(0x50, &buffer_size, 1);
 if(buffer_size>0){
  readBytes(0x51, &dump, 1);
  //Serial.println(dump, HEX);
  if(dump!=lastdump){
    tft.drawRGBBitmap(0, 0, &bitmap2[0], 240, 135);
    lastdump = dump;
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.print(dump, HEX);
    Serial.print("0x");
    Serial.println(dump, HEX);
    if(dump == 0x82){
      analogWrite(TFT_BACKLITE, constrain(backlight_value+=10, 0, 255));
      Serial.println("up");
    }
     if(dump == 0x83){
      analogWrite(TFT_BACKLITE, constrain(backlight_value-=10, 0, 255));
      Serial.println("down");
    }
  }
 }
  

  delay(10);
}