#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h" //include TFT LCD library 
#include "Seeed_FS.h" //include file system library 
#include "SD/Seeed_SD.h"
#include "UI_globals.h"
#include "Pumpe.h"


extern TFT_eSPI tft; //initialize TFT LCD
enum STATE_BOT{boot, home, browseRezept, prepareRezept, doRezept, endRezept, browseFlask_ml, browseFlask_seconds, showTxtExpectations};

class Barbot_StateMachine{
  public:
    int currentRezeptNr = 0;
    int currentFlaskNr = 0;

    float glassWeight = 0;

    float ml_Target = 0;
    float ms_Target = 0;

    STATE_BOT state = boot;
    int rezeptStep = 0;
    int rezeptSubStep = 0;

    float currentWeight = 0;
    
    unsigned long nextWeightCheck = 0.;
    float schrittMS[3][11] = {};
    long runTimeRecipeMS = 0;

    bool paused = false;
    unsigned long displayTime = 0;
    int displayStep = 0;
    bool displayingOld = false;
    int expectationPage = 0;
    unsigned long lastScaleUpdate = 0;

    void loopAll();
    void loopBoot();
    void startHome();
    void loopHome();
    void startBrowseRezept();
    void loopBrowseRezept();
    void startPrepareRezept();
    void loopPrepareRezept();
    void startDoRezept();
    void loopDoRezept();
    void startEndRezept();
    void loopEndRezept();
    void startBrowseFlask_ml();
    void loopBrowseFlask_ml();
    void startBrowseFlask_seconds();
    void loopBrowseFlask_seconds();
    void loopExpectations();
    void startExpectations();
    void loop(){
        switch (state)
        {
        case boot:
          loopBoot();
          break;
        case home:
          loopAll();
          loopHome();
          break;
        case browseRezept:
          loopAll();
          loopBrowseRezept();
          break;
        case prepareRezept:
          loopAll();
          loopPrepareRezept();
          break;
        case doRezept:
          loopAll();
          loopDoRezept();
          break;
        case endRezept:
          loopAll();
          loopEndRezept();
          break;
        case browseFlask_ml:
          loopAll();
          loopBrowseFlask_ml();
          break;
        case browseFlask_seconds:
          loopAll();
          loopBrowseFlask_seconds();
          break;

        case showTxtExpectations:
          loopExpectations();
        default:
          break;
        };
    };
    
};
