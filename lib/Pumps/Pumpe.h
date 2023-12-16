#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h" //include TFT LCD library 
#include "Seeed_FS.h" //include file system library 
#include "SD/Seeed_SD.h"
#include "storageClasses.h"
#include "UI_globals.h"
#include "ConfigAndCommons.h"

class Pumpe{
  public:
    

    bool running=false;
    long runtimeTarget_ms = 0;
    long pumpedMs = 0;
    unsigned long lastStarted = 0;
    unsigned long lastStopped = 0;
    long pumpedUnderFlowProblem = 0;
    bool mayBeFlowProblem = false; 
    int pinNr = 0;
    int pumpNr = 0;
    long PumpedUnderFlowProblem_problem = 0;

    unsigned long lastPumpTimeUpdate = 0;

    float calibratedFlowTarget = 0.f;

    void start();

    void writeHigh_();
    void writeLow_();

    void stop();

    void updatePumpTimes();

    static void flowProblemRectification();

    static void flowProblemStarting(float discrepancy);

    static void pumpStart_recipe(float currentMeasuredWeight, bool resetProblem);

    

    static void pumpStart_blind(float pumpML, float pumpMS, int pumpNr);

    static void _pumpLoop_blind();

    static void pumpLoop(float currentMeasuredWeight);


    static void cancel();

    static void pause();

    static void resume();

    static void parsePumpen();

};


extern unsigned long lastFlowValueDebug;
extern float flowDiscrepancyAverage;
extern float flowTargetAverage;
extern float measurementFlowAverage;
extern float lastMeasurementValue;
extern float discrepancy;
extern unsigned long lastMeasurementTime;
extern long recipeAllPumped;
extern long singleTargetMS;
extern long singlePumped;
extern unsigned long singleStarted;
extern bool blindPumping;
extern int currentRunningNr;
extern int maxRunningNr;
extern float currentFlowTarget;
extern bool flowProblem;
extern unsigned long flowProblemBegan;
extern unsigned long flowProblemEnded;
extern Pumpe pumpen[11];
extern float flowDiscrepancyAtProblemStart;
extern bool donePumping;
extern unsigned long lastPumpStopped;
extern unsigned long lastPumpStarted;
extern float calibrationValue;// = 696.0;; // calibration value (see example file "Calibration.ino")