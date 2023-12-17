#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h" //include TFT LCD library 
#include "Seeed_FS.h" //include file system library 
#include "SD/Seeed_SD.h"
#include "storageClasses.h"

#define HX711_dout BCM2
#define HX711_sck BCM3
#define FLPIN_1 BCM17
#define FLPIN_2 BCM27
#define FLPIN_3 BCM22
#define FLPIN_4 BCM10
#define FLPIN_5 BCM9
#define FLPIN_6 BCM11
#define FLPIN_7 BCM0
#define FLPIN_8 BCM26
#define FLPIN_9 BCM23
#define FLPIN_10 BCM24
#define FLPIN_11 BCM4

#define PIN_MASTER12 WIO_IR
//pins: 11 13 15 19 21 23 27 37 16 18 7
//blocked by buttons: 28, 29, 30, 31, 32, 33, 34, 35, 12



class Barbot_Config{
    public:
      bool loadCellFlipped = true;
      bool pumpLogicInverted[11] = {true,true,false,false,true,true,true,true,false,true, true};
      int msWaitShutoffFlowProblem = 2000;
      int msWaitStartNextPump = 800;
      int maxPumpsRunning = 4;
      float newFlowMeasurementWeight = 0.3;
      float flowProblemThreshold = 0.5;
};

extern Barbot_Config cfg;

extern int nrOfFlasks;
extern int nrOfRecipes;