#include "StateMachine.h"

void Barbot_StateMachine::loopAll(){
  if (LoadCell.update()){ 
      currentWeight = LoadCell.getData();
      if (tareFlag){
        tareFlag=false;
        zeroWeight_tare = currentWeight;
      }
      currentWeight = currentWeight-zeroWeight_tare;
      if (cfg.loadCellFlipped){
        currentWeight = -currentWeight;
      };
      //Serial.println(currentWeight);
  };
  Pumpe::pumpLoop(currentWeight);
  return;
};

void Barbot_StateMachine::loopBoot(){
  startHome();
  return;
};

void Barbot_StateMachine::startHome(){
    state = home;
    startHome_UI();
    lastWeightWasNonZero = millis();
    return;
};
void Barbot_StateMachine::loopHome(){
    loopAll();
    if (currentWeight < (-2.f)){
      tare();
    };
    if(fabs(currentWeight)<0.0001f){
      if(millis()-lastWeightWasNonZero>1000){
        tft.drawString("Check LoadCell!",50,150);
      }
    }
    else{
      lastWeightWasNonZero = millis();
    }
    
    if (currentWeight > 10.f){
      startBrowseRezept();
    };
    
    return;
};
void Barbot_StateMachine::startBrowseRezept(){
  state = browseRezept;
  startChooseRecipe_UI(currentRezeptNr);
  return;
};
void Barbot_StateMachine::loopBrowseRezept(){
    if (digitalRead(WIO_5S_LEFT)==LOW){
      currentRezeptNr = currentRezeptNr-1;
      if (currentRezeptNr <0){currentRezeptNr=nrOfRecipes-1;}
      startChooseRecipe_UI(currentRezeptNr);
      delay(200);
      return;
    }
    if (digitalRead(WIO_5S_RIGHT)==LOW){
      currentRezeptNr = (currentRezeptNr+1)%nrOfRecipes;
      startChooseRecipe_UI(currentRezeptNr);
      delay(200);
      return;
    };
    if (digitalRead(WIO_KEY_B)==LOW){
      startBrowseFlask_ml();
      delay(200);
      return;
    };
    if (digitalRead(WIO_KEY_A)==LOW){
      startHome();
      tare();
      return;
    };
    if (currentWeight < 10.f){
      startHome();
      return;
    };
    if (digitalRead(WIO_5S_PRESS)==LOW){
      startPrepareRezept();
      delay(200);
      return;
    }
    return;
};

void Barbot_StateMachine::startPrepareRezept(){
  state = prepareRezept;
  ml_Target = 150;
  startRecipe_UI(currentRezeptNr, ml_Target);
  redrawWeight_UI(currentWeight);
  return;
};

void Barbot_StateMachine::loopPrepareRezept(){
  if (millis()-displayTime > 200){
    displayTime = millis();
    redrawWeight_UI(currentWeight);
    return;
  };
  if (digitalRead(WIO_5S_DOWN)==LOW){
    ml_Target = ml_Target-20;
    if (ml_Target<20){ml_Target=20;}
    redrawStart_UI(ml_Target);
    delay(200);
    return;
  };
  if (digitalRead(WIO_5S_UP)==LOW){
    ml_Target = ml_Target+20;
    if (ml_Target>360){ml_Target=360;}
    redrawStart_UI(ml_Target);
    delay(200);
    return;
  };
  if (digitalRead(WIO_5S_PRESS)==LOW){
    startDoRezept();
    delay(200);
    return;
  };
  if (currentWeight < 10.f){
        startHome();
    };
  if (digitalRead(WIO_KEY_B)==LOW){
      startBrowseRezept();
      delay(200);
      return;
    };
    return;
  
};
void Barbot_StateMachine::startDoRezept(){
  state = doRezept;
  startBar();
  //parse recipe into pumpMS and prepare Pumps
  runTimeRecipeMS = 0;
  rezeptStep = 0;
  rezeptSubStep = 0;
  glassWeight = currentWeight;
  recipeAllPumped = 0;
  float recipeML = 0;
  if (debug){
    overlayWrite("starting recipe parsing");
    delay(1000);
  }
  for (int stepNr = 0; stepNr < 3; stepNr++){
    if (debug){
      overlayWrite("step "+String(stepNr));
      delay(100);
    }
    for (int flNr = 0; flNr < 12; flNr++){
        float targetML = rezepte[currentRezeptNr].schritt[stepNr][flNr];
        recipeML = recipeML+targetML;
    }
  };
  overlayWrite("Coctail default "+String(recipeML));
  for (int stepNr = 0; stepNr < 3; stepNr++){
    if (debug){
      overlayWrite("step "+String(stepNr));
      delay(100);
    }
    for (int flNr = 0; flNr < 11; flNr++){
        float targetML = rezepte[currentRezeptNr].schritt[stepNr][flNr];
        targetML = targetML/recipeML * ml_Target;
        float targetMS = targetML / pumpen[flNr].calibratedFlowTarget * 1000.f;
        schrittMS[stepNr][flNr] = targetMS;
        runTimeRecipeMS = runTimeRecipeMS+int(targetMS);
    }
  };
  overlayWrite("runtime "+String(runTimeRecipeMS));
  return;
};
void Barbot_StateMachine::loopDoRezept(){
  
  switch (rezeptSubStep){
    case 0:
      for (int flNr = 0; flNr < 11; flNr++){
        pumpen[flNr].runtimeTarget_ms = long(schrittMS[rezeptStep][flNr]);
      }
      if (rezeptStep == 0){
        Pumpe::pumpStart_recipe(currentWeight,true);
        overlayWrite("Startet Recipe");
      }
      else {
        Pumpe::pumpStart_recipe(currentWeight,false);
        overlayWrite("Startet Recipe step");
      }
      rezeptSubStep++;
      break;

    case 1:
      Pumpe::pumpLoop(currentWeight);
      if (donePumping){
        rezeptStep++;
        rezeptSubStep = 0;
        overlayWrite("Pumps done");
      }
      break;    
    }
  if ((millis())-displayTime > 200){
    float percent = float(recipeAllPumped)/float(runTimeRecipeMS)*100.f;
    drawBar(percent);
    displayTime = (millis());
  };
  if ( rezeptStep == 3){
    startEndRezept();
    overlayWrite("Recipe done");
    return;
  }
  if (currentWeight < 10.){
    Pumpe::cancel();
    startHome();
    return;
  }
  if (digitalRead(WIO_KEY_B)==LOW){
    Pumpe::cancel();
    startPrepareRezept();
    delay(200);
    return;
  }
};
void Barbot_StateMachine::startEndRezept(){
  state = endRezept;
  String endText1 = "";
  String endText2 = "";
  bool noError = true;
  float mlDifference = (ml_Target -(currentWeight-glassWeight));
  for (int fl = 0; fl<11; fl++){
            float mlpumped = (pumpen[fl].pumpedMs * pumpen[fl].calibratedFlowTarget) / 1000.f;
            flaschen[fl].fluessigkeit_schaetzung = flaschen[fl].fluessigkeit_schaetzung-mlpumped;
        }
  if (fabs(mlDifference) < 5.f){
      startEndRecipe_UI(currentRezeptNr, String("enjoy!"), String(""));
  }
  else {
    if (mlDifference < 0){
      startEndRecipe_UI(currentRezeptNr, String("enjoy!"), String("hope i did not overfill"));
    }
    else {
        endText1 = String(int(mlDifference))+"ml missing";
        endText2 = String("check");
        for (int fl = 0; fl<11; fl++){
            float ml_under_soll = (pumpen[fl].PumpedUnderFlowProblem_problem * pumpen[fl].calibratedFlowTarget) / 1000.f;
            if (ml_under_soll > 5.f){
              endText2 = endText2+" "+String(flaschen[fl].name).substring(0,flaschen[fl].nameLength);
            }
        }
        startEndRecipe_UI(currentRezeptNr, endText1, endText2);
    }
  }
  displayTime = (millis());  
};
void Barbot_StateMachine::loopEndRezept(){
  if (millis()-displayTime>15000){
    startHome();
    return;
  }
  if (currentWeight < 10.f){
    startHome();
    return;
  }
  if (digitalRead(WIO_5S_PRESS)==LOW){
    startHome();
    return;
  }
};
void Barbot_StateMachine::startBrowseFlask_ml(){
  state = browseFlask_ml;
  ml_Target = 10;
  startFlask_ml_UI(currentFlaskNr, ml_Target);
  
  return;
};
void Barbot_StateMachine::loopBrowseFlask_ml(){
  if(millis()-displayTime > 200){
    displayTime = millis();
    redrawWeight_UI(currentWeight);
  }
  if (currentWeight < 10.f){
      startHome();
      return;
    };
  if (digitalRead(WIO_5S_DOWN)==LOW){
    ml_Target = ml_Target-5;
    if (ml_Target < 5){
      ml_Target = 5;
    }
    redrawStart_UI(ml_Target);
    delay(200);
  }
  if (digitalRead(WIO_5S_UP)==LOW){
    ml_Target = ml_Target+5;
    if (ml_Target > 40){
      ml_Target = 40;
    }
    redrawStart_UI(ml_Target);
    delay(200);
  }
  if (digitalRead(WIO_5S_RIGHT)==LOW){
    currentFlaskNr = (currentFlaskNr+1)%nrOfFlasks;
    startFlask_ml_UI(currentFlaskNr, ml_Target);
    delay(200);
  }
  if (digitalRead(WIO_5S_LEFT)==LOW){
    currentFlaskNr = currentFlaskNr-1;
    if (currentFlaskNr < 0){
      currentFlaskNr = nrOfFlasks-1;
    }
    startFlask_ml_UI(currentFlaskNr, ml_Target);
    if (debug){tft.drawString("flow "+String(pumpen[currentFlaskNr].calibratedFlowTarget),110,110);}
    delay(200);
  }
  if (digitalRead(WIO_5S_PRESS)==LOW){
    Pumpe::pumpStart_blind(ml_Target,0,currentFlaskNr);
    delay(200);
  }
  if (digitalRead(WIO_KEY_C)==LOW){
    flaschen[currentFlaskNr].fluessigkeit_schaetzung = flaschen[currentFlaskNr].fassungsVermoegen_ml;
    startFlask_ml_UI(currentFlaskNr, ml_Target);
    delay(200);
    return;
  }
  if (digitalRead(WIO_KEY_B)==LOW){
    startBrowseRezept();
    delay(200);
    return;
  }
  if (digitalRead(WIO_KEY_A)==LOW){
    startBrowseFlask_seconds();
    delay(200);
    return;
  }
};
void Barbot_StateMachine::startBrowseFlask_seconds(){
  state = browseFlask_seconds;
  ms_Target = 5000;
  startFlask_seconds_UI(currentFlaskNr, int(float(ms_Target)/1000.f));
  return;
};
void Barbot_StateMachine::loopBrowseFlask_seconds(){
  if (currentWeight < 10.f){
      startHome();
      return;
    };
    if(millis()-displayTime > 200){
    displayTime = millis();
    redrawWeight_UI(currentWeight);
  }
  if (digitalRead(WIO_5S_DOWN)==LOW){
    ms_Target = ms_Target-1000;
    if (ms_Target < 1000){
      ms_Target = 1000;
    }
    redrawStart_UI( int(float(ms_Target)/1000.f));
    delay(200);
  }
  if (digitalRead(WIO_5S_UP)==LOW){
    ms_Target = ms_Target+1000;
    if (ms_Target > 60000){
      ml_Target = 60000;
    }
    redrawStart_UI( int(float(ms_Target)/1000.f));
    delay(200);
  }
  if (digitalRead(WIO_5S_RIGHT)==LOW){
    currentFlaskNr = (currentFlaskNr+1)%nrOfFlasks;
    startFlask_seconds_UI(currentFlaskNr, int(float(ms_Target)/1000.f));
    delay(200);
  }
  if (digitalRead(WIO_5S_LEFT)==LOW){
    currentFlaskNr = currentFlaskNr-1;
    if (currentFlaskNr < 0){
      currentFlaskNr = nrOfFlasks-1;
    }
    startFlask_seconds_UI(currentFlaskNr, int(float(ms_Target)/1000.f));
    delay(200);
  }
  if (digitalRead(WIO_5S_PRESS)==LOW){
    Pumpe::pumpStart_blind(0,ms_Target,currentFlaskNr);
    delay(200);
  }
  if (digitalRead(WIO_KEY_C)==LOW){
    flaschen[currentFlaskNr].fluessigkeit_schaetzung = flaschen[currentFlaskNr].fassungsVermoegen_ml;
     startFlask_seconds_UI(currentFlaskNr, int(float(ms_Target)/1000.f));
    delay(200);
    return;
  }
  if (digitalRead(WIO_KEY_B)==LOW){
    startBrowseRezept();
    delay(200);
    return;
  }
  if (digitalRead(WIO_KEY_A)==LOW){
    startBrowseFlask_ml();
    delay(200);
    return;
  }
  return;
};


void Barbot_StateMachine::loopExpectations(){
  if (digitalRead(WIO_5S_LEFT)==LOW){
    expectationPage = expectationPage-1;
    if (expectationPage<0){
      expectationPage = 4;
    }
    showExpectiations_UI(expectationPage);
    delay(200);
    return;
  }
  
  if (digitalRead(WIO_5S_RIGHT)==LOW){
    expectationPage = (expectationPage+1)%5;
    showExpectiations_UI(expectationPage);
    delay(200);
    return;
  }
  return;
};
void Barbot_StateMachine::startExpectations(){
  state = showTxtExpectations;
  showExpectiations_UI(expectationPage);
  return;
};

