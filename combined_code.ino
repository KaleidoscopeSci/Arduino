//Combined greenhouse sensors

#include "DHT.h"
#include <Arduino.h>
#include <U8x8lib.h>
#include <math.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// Define Temp and Humidity sensor
#define DHTPIN 3     // sensor pin
#define DHTTYPE DHT11   // type DHT 11 
DHT dht(DHTPIN, DHTTYPE);

//Intitialize Moisture Sensor
int MoistPin = A0; // Analog input pin for Moisture Sensor
int MoistValue = 0;

//Initialize Light Sensor
int lightPin = A6; // Analog input pin for light sensor
int lightValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(lightPin, INPUT);
  dht.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);
}

void loop() {
  // Measure Temp and Humidity
  float tempC, humi;
  float tempF = 0;
  tempC = dht.readTemperature();
  humi = dht.readHumidity();
  tempF = (tempC * 1.8) + 32; // Convert from C to F

  //Measure Moisture Level
  MoistValue = analogRead(MoistPin);

  //Measure Light Intensity
  lightValue = analogRead(lightPin);

  //Readout on OLED display
  u8x8.setFont(u8x8_font_chroma48medium8_r); // set output on OLED screen

  u8x8.setCursor(0, 0);
  u8x8.print("Temp: ");
  u8x8.print(tempF);
  u8x8.print("F");

  u8x8.setCursor(0, 10);
  u8x8.print("Humidity: ");
  u8x8.print(humi);
  u8x8.print("%");

  u8x8.setCursor(0, 20);
  u8x8.print("Soil: " );
  u8x8.print(MoistValue);
  // Print moisture description
  if (0 <= MoistValue <= 300) {
    u8x8.print(" Dry");
  } else if (300 < MoistValue <= 700) {
    u8x8.print(" Good");
  } else {
    u8x8.print(" Wet");
  }

  u8x8.setCursor(0, 30);
  u8x8.print("Light: ");
  u8x8.print(lightValue);
  // Print light description
  if (lightValue < 360) {
    u8x8.print(" Dim");
  } else if (lightValue > 760) {
    u8x8.print(" High");
  } else {
    u8x8.print(" Good");
  }

  u8x8.refreshDisplay();
  delay(200);

}
