#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h" //include TFT LCD library 
#include "Seeed_FS.h" //include file system library 
#include <HX711_ADC.h>
#include "SD/Seeed_SD.h"
#include "storageClasses.h"
#define HX711_dout BCM2
#define HX711_sck BCM3
#define FLPIN_1 BCM4
#define FLPIN_2 BCM17
#define FLPIN_3 BCM27
#define FLPIN_4 BCM22
#define FLPIN_5 BCM10
#define FLPIN_6 BCM9
#define FLPIN_7 BCM11
#define FLPIN_8 BCM0
#define FLPIN_9 BCM26
#define FLPIN_10 BCM23
#define FLPIN_11 BCM24
//pins: 7 11 13 15 19 21 23 27 37 16 18
//blocked by buttons: 28, 29, 30, 31, 32, 33, 34, 35, 12
//HX711 not working 3,5
extern int pumpenPins[11];

extern bool flowProblem_b;
void startFlowProblem_UI();
void endFlowProblem_UI();

extern HX711_ADC LoadCell;

extern TFT_eSPI tft; //initialize TFT LCD

extern void startHome_UI();

extern void startChooseRecipe_UI(int recipeNr);


extern void startRecipe_UI(int recipeNr, int selectedML);

extern void startFlask_ml_UI(int selectedFlask, int selectedML);

extern void startFlask_seconds_UI(int selectedFlask, int selectedS);

extern void redrawStart_UI(int selected);

extern void redrawWeight_UI(float weight);

extern void drawOverlay(String drawText, int x, int y);
extern void startEndRecipe_UI(int recipeNr, String extraText1, String extraText2);

extern bool debug;
extern char debugOverlay[10][30];
extern int overlayPosition;
extern int overlayPosition_2;
extern void overlayWrite(String next);
extern void _writePosition(int position, bool white);


extern unsigned long toneStart;
extern unsigned long nextToneAction;
extern int tone2play;
extern int tone_duration;
extern bool toneHigh;
extern void toneLoop();

extern float lastPercent;
extern void startBar();
extern void drawBar(float percent);

extern bool loadError;
extern void tare();
extern void showExpectiations_UI(int page);