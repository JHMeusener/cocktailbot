#include <Arduino.h>
#include "StateMachine.h"
#include "Pumpe.h"

bool debug = false;

Pumpe p = Pumpe();
TFT_eSPI tft;
HX711_ADC LoadCell(HX711_dout, HX711_sck);


unsigned long zeroValue = 0;


Barbot_StateMachine bot = Barbot_StateMachine();

bool mock = false;

void setup() {
  Serial.begin(115200);
  if (!mock){LoadCell.begin();}
  pinMode(WIO_KEY_A, INPUT); //set button A pin as input         28
  pinMode(WIO_KEY_B, INPUT); //set button B pin as input         29
  pinMode(WIO_KEY_C, INPUT); //set button C pin as input         30
  pinMode(WIO_5S_UP, INPUT); //set switch pin up as input        31
  pinMode(WIO_5S_DOWN, INPUT); //set switch pin down as input    34
  pinMode(WIO_5S_LEFT, INPUT); //set switch pin left as input    32
  pinMode(WIO_5S_RIGHT, INPUT); //set switch pin right as input  33
  pinMode(WIO_5S_PRESS, INPUT); //set switch pin press as input  35
  pinMode(WIO_BUZZER, OUTPUT); //set buzzer pin as output        12
  for (int i = 0; i<11; i++){
    pumpen[i].pinNr = pumpenPins[i];
    pinMode(pumpenPins[i], OUTPUT);
  }

  tft.begin(); //start TFT LCD
  tft.setRotation(3); //set screen rotation 
  tft.fillScreen(TFT_BLACK); //fill background 
  //tft.setFreeFont(&FreeSansBold12pt7b); //set font type 
  tft.setTextColor(TFT_WHITE); //set text color 
  tft.setTextSize(1); //set text size
  if (!mock){
      bot.startExpectations();
      while (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)){ //check whether SD card is inserted and working 
        tft.drawString("No SD-Card found",150,150);
        bot.loop();
      }

      LoadCell.startMultiple(10000, true);
      //Drawing for brightness
      if (LoadCell.getTareTimeoutFlag()) {
        while(true){
          tft.drawString("Check HX711",150,150);
          bot.loop();
        }
      }
      else {
        
        overlayWrite("loadCell started");
      }
  }
  tft.fillScreen(TFT_BLACK);
  overlayWrite("loading Flasks");
  for (int i=0;i<11;i++){
    flaschen[i].parseFlasche(String(i));
    tft.setCursor(5,5+i*10);
  }
  
  overlayWrite("loading Recipes");
  for (int i=0;i<35;i++){
    rezepte[i].parseRezept(String(i));
  }
  delay(500);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5,5);

  overlayWrite("loading PumpConfig");
  Pumpe::parsePumpen();
  LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
  delay(500);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5,5);
  SD.end();
  if (loadError){
    while(true){
          tft.drawString("Some files not loaded",150,150);
          bot.loop();
        }
  }
  else {
    bot.startHome();
  }
}







bool newDataReady = false;
void loop() {
  bot.loop();
}

