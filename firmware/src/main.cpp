#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "key_config.h"
#include "defines.h"
#include <queue>

#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
std::queue<uint8_t> pressQueue;
uint8_t backlight_value = 125;

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  delay(500); // Wait for devices to settle (adjust the delay as needed)
  

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  pinMode(TFT_BACKLITE, ANALOG);
  analogWrite(TFT_BACKLITE, backlight_value);
  delay(10);

  // Init 240x135 TFT
  tft.init(135, 240);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  Keyboard.begin();
  USB.begin();
}

void testI2C();

void readBytes(uint8_t addr, uint8_t *data, uint8_t size);

uint8_t dump, lastdump;

void loop()
{
  
  testI2C();
  delay(10);
}

void readBytes(uint8_t addr, uint8_t *data, uint8_t size)
{
  Wire.beginTransmission(FIRST_MINICLICK_ADDRESS);

  Wire.write(addr);
  uint8_t err = Wire.endTransmission(false);
  if (err != ESP_OK)
  {
    Serial.println(err);
  }
  Wire.requestFrom((uint16_t)FIRST_MINICLICK_ADDRESS, size);
  for (uint8_t i = 0; i < size; ++i)
  {
    data[i] = Wire.read();
  }
}

void testI2C(){
  byte error, address;
  int deviceCount = 0;
  uint8_t buffer_size = 0;
  readBytes(miniclick_device_queue_size_address, &buffer_size, 1);
  if (buffer_size > 0)
  {
    readBytes(miniclick_pop_queue_address, &dump, 1);

    if (dump != lastdump)
    {
      tft.drawRGBBitmap(0, 0, &bitmap2[0], 240, 135);
      lastdump = dump;
      tft.setCursor(10, 10);
      tft.setTextColor(ST77XX_GREEN);
      tft.setTextSize(3);
      tft.print("0x");
      tft.print(dump, HEX);

      if(BUTTON_PRESSED | 0x01){
        Keyboard.write('P');
      }
    }
  }
}