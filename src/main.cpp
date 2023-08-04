#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7789.h>
#include <SPI.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int scanPin = 45;

void setup() {
  Wire.begin(42, 41);
  Serial.begin(115200);
  pinMode(scanPin, OUTPUT);
  digitalWrite(scanPin, LOW);  // Start with pin 45 low
  delay(500);                  // Wait for devices to settle (adjust the delay as needed)
  pinMode(21, OUTPUT);         //i2c power
  digitalWrite(21, HIGH);
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

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

void loop() {
  byte error, address;
  int deviceCount = 0;


  
  readBytes(0x51, &dump, 1);
  //Serial.println(dump, HEX);
  if(dump!=lastdump && dump!=0xFF){
    lastdump = dump;
     tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.print(dump, HEX);
    Serial.print("0x");
    Serial.println(dump, HEX);
  }
 

  delay(50);  // Wait 5 seconds before scanning again (adjust the delay as needed)
}