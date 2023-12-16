#include <Pumpe.h>
void Pumpe::start(){
      if (running){ return;}
      currentRunningNr = currentRunningNr+1;
      running = true;
      writeHigh_();
      lastStarted = millis();
      lastPumpTimeUpdate = millis();
      lastPumpStarted = millis();
    };

void Pumpe::stop(){
      if (!running){
        return;}
      currentRunningNr = currentRunningNr-1;
      
      if (fabs(currentFlowTarget)<0.01f){currentFlowTarget = 0.f;}
      writeLow_();
      running = false;
      lastStopped = millis();
      lastPumpStopped = millis();
      pumpedMs = pumpedMs+long(lastStopped-lastPumpTimeUpdate);
      if (flowProblem){
        pumpedUnderFlowProblem = pumpedUnderFlowProblem + long(lastStopped-lastPumpTimeUpdate);
      }
      lastPumpStopped = lastStopped;
    };

void Pumpe::writeHigh_(){
  if (cfg.pumpLogicInverted[pumpNr]){
    digitalWrite(pinNr, LOW);
  }
  else {
    digitalWrite(pinNr, HIGH);
  }
  return;

};
void Pumpe::writeLow_(){
  if (cfg.pumpLogicInverted[pumpNr]){
    digitalWrite(pinNr, HIGH);
  }
  else {
    digitalWrite(pinNr, LOW);
  }
  return;
};

void Pumpe::updatePumpTimes(){
      if (!running){ 
        return;
        }
      unsigned long currentTime = millis();
      if ((millis()-lastPumpTimeUpdate) < 50){ return;}
      long additionalMS = long(currentTime-lastPumpTimeUpdate);
      lastPumpTimeUpdate = millis();
      pumpedMs = pumpedMs+additionalMS;
      recipeAllPumped = recipeAllPumped+additionalMS;
      if (flowProblem){
        pumpedUnderFlowProblem = pumpedUnderFlowProblem + additionalMS;
      }
      
    };

void Pumpe::flowProblemRectification(){
        flowProblemEnded = millis();
        flowProblem = false;
        if ((flowProblemEnded-flowProblemBegan) > 999999){return;} //no flow problem
        long flowProblemTime = flowProblemEnded-flowProblemBegan;
        
        long mostLikelyFaultyPump = -1;
        long lowestResolveTimeAfterShutoff = 999999;
        int nrOfFaultCandidates = 0;
        for (int i=0; i<11; i++){

          //exclude unlikely candidates        
          if (pumpen[i].running){                     //pump running and problem is gone
            pumpen[i].mayBeFlowProblem = false;
            pumpen[i].PumpedUnderFlowProblem_problem = 0;
            pumpen[i].pumpedUnderFlowProblem = 0;
            continue;
          }
          if (flowProblemBegan - pumpen[i].lastStarted > 999999){ //problem existed before pump started
            pumpen[i].mayBeFlowProblem = false;
            pumpen[i].pumpedUnderFlowProblem = 0;
            pumpen[i].PumpedUnderFlowProblem_problem = 0;
            continue;
          }
          if (!pumpen[i].mayBeFlowProblem){
            pumpen[i].pumpedUnderFlowProblem = 0;
            pumpen[i].PumpedUnderFlowProblem_problem = 0;
            continue;
          }

          if (flowProblemEnded-pumpen[i].lastStopped  < 200){
            pumpen[i].mayBeFlowProblem = false;
            pumpen[i].pumpedUnderFlowProblem = 0;
            pumpen[i].PumpedUnderFlowProblem_problem = 0;
            continue;
          }

          if (flowProblemEnded-pumpen[i].lastStopped > (cfg.msWaitShutoffFlowProblem+500)){
            pumpen[i].mayBeFlowProblem = false;
            pumpen[i].pumpedUnderFlowProblem = 0;
            pumpen[i].PumpedUnderFlowProblem_problem = 0;
            continue;
          }

          if ((flowProblemEnded-pumpen[i].lastStopped) < lowestResolveTimeAfterShutoff){
            mostLikelyFaultyPump = i;
            lowestResolveTimeAfterShutoff = pumpen[i].lastStopped - flowProblemEnded;
          }

          pumpen[i].PumpedUnderFlowProblem_problem = pumpen[i].PumpedUnderFlowProblem_problem + pumpen[i].pumpedUnderFlowProblem;
          nrOfFaultCandidates++;

      }
      if (nrOfFaultCandidates==1){
        //skip this pump in step
        if ( pumpen[mostLikelyFaultyPump].runtimeTarget_ms -  pumpen[mostLikelyFaultyPump].pumpedMs > 0){
           pumpen[mostLikelyFaultyPump].PumpedUnderFlowProblem_problem =  pumpen[mostLikelyFaultyPump].PumpedUnderFlowProblem_problem +  pumpen[mostLikelyFaultyPump].runtimeTarget_ms -  pumpen[mostLikelyFaultyPump].pumpedMs;
           pumpen[mostLikelyFaultyPump].pumpedMs =  pumpen[mostLikelyFaultyPump].runtimeTarget_ms;
        }
      }
    };

void Pumpe::flowProblemStarting(float discrepancy){
        flowProblemBegan = millis();
        flowProblem = true;
        flowDiscrepancyAtProblemStart = discrepancy;

        for (int i=0; i<11; i++){
          
          if (pumpen[i].running){
            pumpen[i].mayBeFlowProblem = true;
          }
        }
        
    };


void Pumpe::pumpStart_recipe(float currentMeasuredWeight, bool resetProblem){
      flowDiscrepancyAverage = 0;
      flowTargetAverage = 0;
      measurementFlowAverage = 0;
      lastMeasurementValue = currentMeasuredWeight;
      discrepancy = 0;
      lastMeasurementTime = millis();
      donePumping = false;
      long ms_all = 0;
      for (int i=0; i<11; i++){
          
          pumpen[i].pumpedUnderFlowProblem = 0;
          ms_all = ms_all+pumpen[i].runtimeTarget_ms;
          pumpen[i].pumpedMs = 0;
          pumpen[i].lastPumpTimeUpdate = millis();
          if (resetProblem){
            pumpen[i].PumpedUnderFlowProblem_problem = 0;
          }
          pumpen[i].updatePumpTimes();
        }
      maxRunningNr = cfg.maxPumpsRunning;
      flowProblem = false;
      if ( ms_all == 0){
        donePumping = true;
      }
    };

void Pumpe::pumpStart_blind(float pumpML, float pumpMS, int pumpNr){
      overlayWrite("pumpStart_blind");
      blindPumping = true;
      singleStarted = millis();
      
      for (int i=0; i<11; i++){
        if (i == pumpNr){
          pumpen[i].writeHigh_();
          if (pumpMS > 0){
            singleTargetMS = pumpMS;
            flaschen[pumpNr].fluessigkeit_schaetzung = flaschen[pumpNr].fluessigkeit_schaetzung-(pumpMS*pumpen[pumpNr].calibratedFlowTarget /1000.f);
          }
          if (pumpML > 0){
            singleTargetMS = int(pumpML/max(1.f,pumpen[i].calibratedFlowTarget) * 1000.f);
            flaschen[pumpNr].fluessigkeit_schaetzung = flaschen[pumpNr].fluessigkeit_schaetzung-(singleTargetMS*pumpen[pumpNr].calibratedFlowTarget /1000.f);
          }
        }
        }
    };

void Pumpe::_pumpLoop_blind(){
      if (millis()-singleStarted > singleTargetMS){
        for (int i= 0; i<11;i++){
          pumpen[i].writeLow_();
          overlayWrite("pump_blind ended");
        }
        blindPumping = false;
      }
    };

unsigned long lastFlowValueDebug = 0;

void Pumpe::pumpLoop(float currentMeasuredWeight){
        //update flowMeasurementTimes
        if ( millis()-lastMeasurementTime > 200){
            currentFlowTarget = 0;
            currentRunningNr = 0;
            for (int flNr = 0; flNr < 11; flNr++){
                if (pumpen[flNr].running){
                  currentFlowTarget = currentFlowTarget+pumpen[flNr].calibratedFlowTarget;
                  currentRunningNr = currentRunningNr+1;
                }
            }
            float currentMeasuredFlowDT = float(millis()-lastMeasurementTime);
            float currentMeasuredWeightDT = currentMeasuredWeight-lastMeasurementValue;
            float currentMeasuredFlow = currentMeasuredWeightDT/currentMeasuredFlowDT;
            lastMeasurementTime = millis();
            lastMeasurementValue = currentMeasuredWeight;
            measurementFlowAverage = measurementFlowAverage*(1.f-cfg.newFlowMeasurementWeight) + cfg.newFlowMeasurementWeight * currentMeasuredFlow;
            flowTargetAverage = flowTargetAverage*(1.f-cfg.newFlowMeasurementWeight) + cfg.newFlowMeasurementWeight*currentFlowTarget;
            float discepancy = measurementFlowAverage-flowTargetAverage;
            flowDiscrepancyAverage = flowDiscrepancyAverage*(1.f-cfg.newFlowMeasurementWeight) + cfg.newFlowMeasurementWeight*discepancy;
            drawOverlay("Flow:"+String(measurementFlowAverage), 5, 5);
            drawOverlay("trgt:"+String(flowTargetAverage), 75, 5);
            drawOverlay("disc:"+String(flowDiscrepancyAverage), 150, 5);
            drawOverlay("fProblem:"+String(flowProblem), 225, 5);
            drawOverlay("runningNr "+String(currentRunningNr)+" max "+String(maxRunningNr), 5, 25);
            
        }   
        
        //check for FlowProblem existence
        if (fabs(flowDiscrepancyAverage) < pumpen[0].calibratedFlowTarget*cfg.flowProblemThreshold){
          //no flow Problem
          if (flowProblem){
            flowProblemRectification();
            if (!donePumping){
              endFlowProblem_UI();
            }
          }
        }
        else {
          if (flowDiscrepancyAverage<0){
            //flowProblem
            if (!flowProblem){
                flowProblemStarting(flowDiscrepancyAverage);
                if (!donePumping){
                  startFlowProblem_UI();
                }
            }
          }
        }

        if(blindPumping){
          _pumpLoop_blind();
          drawOverlay("BLIND PUMPING", 150, 150);
          return;
        }

        if (donePumping){
          drawOverlay("DONE PUMPING", 150, 150);
          drawOverlay("            ", 150, 150);
          return;
        }

        for (int i=0; i<11; i++){
          pumpen[i].updatePumpTimes();
          if (pumpen[i].runtimeTarget_ms != 0){
                drawOverlay(String(i), i*35+5, 35);
                drawOverlay("target", i*35+5, 55);
                drawOverlay(String(pumpen[i].runtimeTarget_ms), i*35+5, 75);
                drawOverlay(String(pumpen[i].pumpedMs), i*35+5, 95);
                drawOverlay("running", i*35+5, 115);
                drawOverlay(String(pumpen[i].running), i*35+5, 135);
          }
        }


        if (!flowProblem){
            //NORMAL FLOW: check if pumps can be switched on
            if (millis()-lastPumpStarted > cfg.msWaitStartNextPump){
              if (currentRunningNr < maxRunningNr){
                  drawOverlay("can start pump", 210, 50);
                  int highestMsTargetRemaining = 0;
                  int pumpNrHighest = 0;

                  int highestMsTargetRemaining_notPumping = 0;
                  int pumpNrHighest_notPumping = 0;

                  for (int i=0; i<11; i++){
                    if ((pumpen[i].runtimeTarget_ms - pumpen[i].pumpedMs) > highestMsTargetRemaining){
                      highestMsTargetRemaining = pumpen[i].runtimeTarget_ms - pumpen[i].pumpedMs;
                      pumpNrHighest = i;
                    }
                    if (!pumpen[i].running){
                      if ((pumpen[i].runtimeTarget_ms - pumpen[i].pumpedMs) > highestMsTargetRemaining_notPumping){
                        highestMsTargetRemaining_notPumping = pumpen[i].runtimeTarget_ms - pumpen[i].pumpedMs;
                        pumpNrHighest_notPumping = i;
                      }
                    }
                  };
                  if (highestMsTargetRemaining < 1){
                      donePumping = true;
                      return;
                  }
                  if (highestMsTargetRemaining_notPumping > 0){
                      pumpen[pumpNrHighest_notPumping].start();
                  }
              }
            }
        }
      else {
          //FLOWPROBLEM PRESENT: check if pumps can be switched off
          if (millis()-lastPumpStopped > cfg.msWaitShutoffFlowProblem){
            if (currentRunningNr > 0){
                int lowestRemainingTime = 99999;
                int lowestRemainingTimePumpNr = 0;
                for (int i= 0; i<11;i++){
                  
                  if (pumpen[i].running){
                    int remaining = pumpen[i].runtimeTarget_ms-pumpen[i].pumpedMs;
                    if (lowestRemainingTime > remaining){
                      lowestRemainingTime = remaining;
                      lowestRemainingTimePumpNr = i;
                    }
                  }
                }
                if ((lowestRemainingTime < 999999)&&(lowestRemainingTime > 2000)&&(currentRunningNr>0)){
                    pumpen[lowestRemainingTimePumpNr].stop();
                }
            }
        };
      };

      //check if pump is done
      for (int i= 0; i<11;i++){
         
          if (pumpen[i].running){
            int remaining = pumpen[i].runtimeTarget_ms-pumpen[i].pumpedMs;
            if (remaining < 0){
              pumpen[i].stop();
              drawOverlay("Finished", i*15+5, 155);
            }
          }
      }

    };


void Pumpe::cancel(){
        for (int i= 0; i<11;i++){
          
          pumpen[i].runtimeTarget_ms = pumpen[i].pumpedMs;
        }
        singleStarted = millis()-singleTargetMS;
    };

void Pumpe::pause(){
      maxRunningNr = 0;
      for (int i= 0; i<11;i++){
          
          pumpen[i].stop();
          pumpen[i].writeLow_();
      }
    };

void Pumpe::resume(){
      maxRunningNr = cfg.maxPumpsRunning;
      donePumping = false;
    };


unsigned long lastPumpStarted = 0;

float calibrationValue = 696.0;// calibration value (see example file "Calibration.ino")
void Pumpe::parsePumpen(){
        File myFile = SD.open("pumpenMLperSecond.txt"); // open the file for reading
        if (myFile) { 
          overlayWrite("found pumpenMLperSecond.txt");
          overlayWrite("expecting: mlPerSecond");
          overlayWrite("           values");
          int lineNr = 0;
          while (myFile.available()) { //execute while file is available
              String line = myFile.readStringUntil('\n');  // \n character is discarded from buffer
              if (line.indexOf("#") > 0){
                line = line.substring(0,line.indexOf("#"));
              }
              overlayWrite(String(lineNr)+": "+line);
              if (lineNr > 0){
                pumpen[lineNr-1].calibratedFlowTarget = line.toFloat();
              }
              if (lineNr == calibrationValue){
                calibrationValue = line.toFloat();
              }
              
              lineNr++;
              if (lineNr > 11){
                break;
              }
          }
          myFile.close(); //close file
        }
        else {
          overlayWrite("missing:");
          overlayWrite("pumpenMLperSecond.txt");
          delay(1000);
          loadError = true;
        }
    };


int currentRunningNr = 0;
int maxRunningNr = cfg.maxPumpsRunning;
float currentFlowTarget = 0;
bool flowProblem = 0;
unsigned long flowProblemBegan = 0;
unsigned long flowProblemEnded = 0;
float flowDiscrepancyAtProblemStart = 0;
bool donePumping = true;
unsigned long lastPumpStopped = 0;
float flowDiscrepancyAverage = 0;
float flowTargetAverage = 0;
float measurementFlowAverage = 0;
float lastMeasurementValue = 0;
float discrepancy = 0;
unsigned long lastMeasurementTime = 0;
long recipeAllPumped = 0;
long singleTargetMS = 0;
long singlePumped = 0;
unsigned long singleStarted = 0;
bool blindPumping = 0;

Pumpe pumpen[11] = {Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe(),Pumpe()};