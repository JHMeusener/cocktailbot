#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h" //include TFT LCD library 
#include "Seeed_FS.h" //include file system library 
#include <HX711_ADC.h>
#include "SD/Seeed_SD.h"
#include "storageClasses.h"
#include "ConfigAndCommons.h"
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
extern bool tareFlag;
extern void showExpectiations_UI(int page);