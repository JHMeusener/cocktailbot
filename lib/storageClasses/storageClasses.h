#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h" //include TFT LCD library 
#include "Seeed_FS.h" //include file system library 
#include <HX711_ADC.h>
#include "SD/Seeed_SD.h"
#include "UI_globals.h"

extern TFT_eSPI tft; //initialize TFT LCD
class Flasche{
  public:
    float fassungsVermoegen_ml = 750.;
    float fluessigkeit_schaetzung = 750.;
    char name[15] = {};
    int nameLength = 0;
    
    bool parseFlasche(String filename);

};


extern Flasche flaschen[12];

class Rezept{
  public:
    char name[20] = {};
    char shortText1[20] = {};
    char shortText2[20] = {};
    char shortText3[20] = {};
    char endText1[20] = {};
    char endText2[20] = {};
    int nameLength = 0;
    int shortTextLength1 = 0;
    int shortTextLength2 = 0;
    int shortTextLength3 = 0;
    int endTextLength1 = 0;
    int endTextLength2 = 0;
    float schritt[3][12] = {}; //maxpumps + dummy
    
    bool parseRezept(String filename);
};


extern Rezept rezepte[35];
