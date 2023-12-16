#include <UI_globals.h>
int pumpenPins[11] = {FLPIN_1,FLPIN_2,FLPIN_3,FLPIN_4,FLPIN_5,FLPIN_6,FLPIN_7,FLPIN_8,FLPIN_9,FLPIN_10,FLPIN_11};


char debugOverlay[10][30] = {};
int overlayPosition = 0;
unsigned long overlayWait = 0;
int overlayPosition_2 = 0;
void overlayWrite(String next){
  if (!debug){return;}
  int oldTextColor = tft.textcolor;
  int oldTextSize = tft.textsize;
  int oldposX = tft.cursor_x;
  int oldposy = tft.cursor_y;

  tft.setTextColor(TFT_RED);
  tft.setTextSize(1);
  tft.drawString(next,5,5+15*overlayPosition);
  if (overlayPosition_2%10 == 0){
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(5,5);
  }
  tft.setTextColor(oldTextColor);
  tft.setTextSize(oldTextSize);
  //tft.setCursor(oldposX,oldposy);
  overlayPosition++;
  overlayPosition_2++;
  overlayPosition=overlayPosition%10;
  
  return;
};
  

void _writePosition(int position, bool white){
  int oldTextColor = tft.textcolor;
  int oldTextSize = tft.textsize;
  int oldposX = tft.cursor_x;
  int oldposy = tft.cursor_y;
  if (white){
    tft.setTextColor(TFT_WHITE);
  }
  else {
    tft.setTextColor(TFT_RED);
  }
  tft.setTextSize(1);

  tft.setCursor(oldposX,oldposy);
  tft.drawString(String(debugOverlay[position]),5,15*position+25);
  
  tft.setTextColor(oldTextColor);
  tft.setTextSize(oldTextSize);
  tft.setCursor(oldposX,oldposy);
  return;
}





void startHome_UI(){
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN); //set text color 
    tft.setTextSize(4); //set text size
    tft.drawString("BarBot",3,3); //draw text string 
    tft.drawRect(5,40,315,40,TFT_RED);
    tft.setTextSize(3);
    tft.drawString("Position Glass",10,50);
    return;
};

void startChooseRecipe_UI(int recipeNr){
  tft.fillScreen(TFT_WHITE); //fill background 
  //tft.setFreeFont(&FreeSansBold12pt7b); //set font type 
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.setTextSize(2); //set text size
  tft.drawString("      flasks zero-scale",3,3); //draw text string 
  //tft.drawString("select <o>",205,220);
  tft.drawString("select <o>",205,220);
  tft.drawRect(5,80,315,120,TFT_LIGHTGREY);
  tft.drawRect(5,40,315,40,TFT_RED);
  tft.setTextSize(3);
  tft.drawString(String(rezepte[recipeNr].name).substring(0,rezepte[recipeNr].nameLength),10,50); //draw text string 
  tft.setTextSize(2);
  tft.drawString(String(rezepte[recipeNr].shortText1).substring(0,rezepte[recipeNr].shortTextLength1),10,90); //draw text string 
  tft.drawString(String(rezepte[recipeNr].shortText2).substring(0,rezepte[recipeNr].shortTextLength2),10,130); //draw text string 
  tft.drawString(String(rezepte[recipeNr].shortText3).substring(0,rezepte[recipeNr].shortTextLength3),10,170); //draw text string 
  tft.drawString("Nr "+String(recipeNr+1)+" von "+String(nrOfRecipes),40,210); //draw text string 
  return;
};


void startRecipe_UI(int recipeNr, int selectedML){
  tft.fillScreen(TFT_WHITE); //fill background 
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.drawString("      cancel",3,3); //draw text string 
  tft.drawString("select  o",205,220); tft.drawString("\\/",294,228); tft.drawString("/\\",294,212);
  tft.drawRect(5,60,315,40,TFT_LIGHTGREY);
  tft.setTextSize(3);
  tft.drawString(String(rezepte[recipeNr].name).substring(0,rezepte[recipeNr].nameLength),10,70); //draw text string 
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.drawString("how many ml?",10,130);
  tft.setTextSize(3);
  tft.drawString(String(selectedML), 190, 130);
  tft.setTextSize(2);
  tft.drawString("ml",280,140);
  tft.drawRect(180,120,90,50,TFT_RED);
  return;
};

void startFlask_ml_UI(int selectedFlask, int selectedML){
  tft.fillScreen(TFT_WHITE); //fill background 
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.drawString("refilled cancel seconds",3,3); //draw text string 
  tft.drawString("select <o>",205,220); tft.drawString("\\/",294,228); tft.drawString("/\\",294,212);
  tft.drawRect(5,60,315,40,TFT_LIGHTGREY);
  tft.setTextSize(3);
  tft.drawString(String(flaschen[selectedFlask].name).substring(0,flaschen[selectedFlask].nameLength),10,70); //draw text string 
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.drawString("how many ml?",10,130);
  tft.setTextSize(3);
  tft.drawString(String(selectedML), 190, 130);
  tft.setTextSize(2);
  tft.drawString("ml",280,140);
  tft.drawRect(180,120,90,50,TFT_RED);
  tft.drawString("Est "+String(int(flaschen[selectedFlask].fluessigkeit_schaetzung))+"of"+String(int(flaschen[selectedFlask].fassungsVermoegen_ml))+" ml",40,180); //draw text string 
  tft.drawString("Nr "+String(selectedFlask+1)+" von 11",40,210); //draw text string 
  return;
};

void startFlask_seconds_UI(int selectedFlask, int selectedS){
  tft.fillScreen(TFT_WHITE); //fill background 
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.drawString("refilled cancel ml",3,3); //draw text string 
  tft.drawString("select <o>",205,220); tft.drawString("\\/",294,228); tft.drawString("/\\",294,212);
  tft.drawRect(5,60,315,40,TFT_LIGHTGREY);
  tft.setTextSize(3);
  tft.drawString(String(flaschen[selectedFlask].name).substring(0,flaschen[selectedFlask].nameLength),10,70); //draw text string 
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.drawString("how many sec?",10,130);
  tft.setTextSize(3);
  tft.drawString(String(selectedS), 190, 130);
  tft.setTextSize(2);
  tft.drawString("sec",280,140);
  tft.drawRect(180,120,90,50,TFT_RED);
  tft.drawString("Est "+String(int(flaschen[selectedFlask].fluessigkeit_schaetzung))+"of"+String(int(flaschen[selectedFlask].fassungsVermoegen_ml))+" ml",40,180); //draw text string 
  tft.drawString("Nr "+String(selectedFlask+1)+" von 11",40,210); //draw text string 
  return;
};

void redrawStart_UI(int selected){
  tft.fillRect(180,120,90,50,TFT_WHITE);
  tft.drawRect(180,120,90,50,TFT_RED);
  tft.setTextSize(3);
  tft.drawString(String(selected), 190, 130);
  tft.setTextSize(2);
  return;
};

void redrawWeight_UI(float weight){
  tft.fillRect(40,150,100,30,TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString(String(weight), 40, 150);
  tft.drawString("g", 140, 150);
  tft.setTextSize(2);
  return;
};



unsigned long toneStart = 0;
unsigned long nextToneAction = 0;
int tone2play = 0;
int tone_duration = 0;
bool toneHigh = false;
void toneLoop() {
    if (micros() < toneStart+(tone_duration*1000)){
        if (micros() > nextToneAction){
            nextToneAction = micros()+500000/tone2play;
            if (toneHigh){
              toneHigh = false;
              digitalWrite(WIO_BUZZER, LOW);
            }
            else{
              toneHigh = true;
              digitalWrite(WIO_BUZZER, HIGH);
            }
        }
    }
    return;
};

float lastPercent = 0;
void startBar(){
  //tft.fillRect(10,140,300,60,TFT_WHITE); //fill rectangle with color 
  tft.drawRect(10,140,300,60,TFT_NAVY); //draw rectangle with border
  tft.drawRect(11,141,298,58,TFT_NAVY); //draw rectangle with border
  lastPercent = 0;
};
void drawBar(float percent){
  if (percent > 100.f){
    percent = 100.f;
  }
  if (percent < lastPercent){percent = lastPercent;}
  if (lastPercent > 99.9f){
    return;
  }
  if (flowProblem_b){
    tft.fillRect(10+max(0,int(lastPercent/100.f*300.f)-5), 140, int(300.f*percent/100.f)-int(lastPercent/100.f*300.f), 20, TFT_NAVY);
    tft.fillRect(10+max(0,int(lastPercent/100.f*300.f)-5), 180, int(300.f*percent/100.f)-int(lastPercent/100.f*300.f), 20, TFT_NAVY);
  }
  else{
    tft.fillRect(10+max(0,int(lastPercent/100.f*300.f)-5), 140, int(300.f*percent/100.f)-int(lastPercent/100.f*300.f), 60, TFT_NAVY);
  }
  
  lastPercent = percent;
  return;
};

bool tareFlag = false;
void tare(){
  tareFlag=true;
}

bool flowProblem_b = false;
void startFlowProblem_UI(){
  flowProblem_b = true;
  tft.fillRect(30, 160, 260, 20, TFT_WHITE);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.drawString("Flowproblem",60,160);
};
void endFlowProblem_UI(){
  flowProblem_b = false;
  tft.fillRect(30, 160, 260, 20, TFT_WHITE);
  tft.fillRect(10, 140, int(300.f*lastPercent/100.f), 60, TFT_NAVY);
};


void startEndRecipe_UI(int recipeNr, String extraText1, String extraText2){
  tft.fillScreen(TFT_WHITE); //fill background 
  //tft.setFreeFont(&FreeSansBold12pt7b); //set font type 
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.setTextSize(2); //set text size
  //tft.drawString("select <o>",205,220);
  tft.drawString("press ---> ",205,220);
  tft.drawRect(5,110,315,100,TFT_LIGHTGREY);
  tft.drawRect(5,40,315,70,TFT_RED);
  tft.setTextSize(2);
  if (extraText1.length() > 0){
    tft.drawString(extraText1,10,50); //draw text string 
  }
  if (extraText2.length() > 0){
    tft.drawString(extraText2,10,80); //draw text string 
  }
  tft.drawString(String(rezepte[recipeNr].endText1).substring(0,rezepte[recipeNr].endTextLength1),10,115); //draw text string 
  tft.drawString(String(rezepte[recipeNr].endText2).substring(0,rezepte[recipeNr].endTextLength2),10,145); //draw text string 
  tft.drawString(String(rezepte[recipeNr].shortText3).substring(0,rezepte[recipeNr].shortTextLength3),10,175); //draw text string 
  return;
};

void drawOverlay(String drawText, int x, int y){
  if (!debug){return;}
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.setTextSize(1); //set text size
  int length = drawText.length();
  tft.fillRect(x,y,5*length+30,15,TFT_WHITE);
  tft.drawString(drawText,x,y);
  return;
};

bool loadError = false;

void showExpectiations_UI(int page){
  tft.fillScreen(TFT_WHITE); //fill background 
  //tft.setFreeFont(&FreeSansBold12pt7b); //set font type 
  tft.setTextColor(TFT_BLACK); //set text color 
  tft.setTextSize(1); //set text size
  //tft.drawString("select <o>",205,220);
  tft.drawString("Page "+String(page+1)+" of 5",5,220);
  tft.drawString("select <o>",205,220);
  tft.drawRect(5,5,315,30,TFT_RED);
  switch (page){
      case 0:
          tft.drawString("Filesystem",50,10);
          tft.drawString("rezept1.txt",5,40);
          tft.drawString("...",15,60);
          tft.drawString("rezept35.txt   <- mindestens 1",5,80);
          tft.drawString("flasche1.txt",5,120);
          tft.drawString("...",15,140);
          tft.drawString("flasche11.txt   <- mindestens 1",5,160);
          tft.drawString("pumpenMLperSecond.txt  <- muss",5,200);
          break;
      case 1:
          tft.drawString("rezept1.txt",50,10);
          tft.drawString("Tequila Sunrise  # name",5,40);
          tft.drawString("leckerer Cocktail  # beschreibung 1",5,60);
          tft.drawString("mit O-Saft  # beschreibung 2",5,80);
          tft.drawString(" fuelle vorsichtig  # endtext1",5,100);
          tft.drawString("mit O-Saft auf  # endtext1",5,120);
          tft.drawString("Grenadine-15 # flaskname-ml",5,140);
          tft.drawString("-----  # new step (up to 3)",5,160);
          tft.drawString("Tequila-25 # flaskname-ml",5,180);
          tft.drawString("Limettensaft-5  # flaskname-ml",5,200);
          break;
      case 2:
          tft.drawString("flasche1.txt",50,10);
          tft.drawString("Tequila # name",5,40);
          tft.drawString("750 # fassungsvermoegen der Flasch in mL ",5,60);
          tft.drawString("250 # aktuelle Fuellung in ml ",5,80);
          break;
      case 3:
          tft.drawString("pumpenMLperSecond.txt",50,10);
          tft.drawString("696.0  # calibrationValue of Scale",5,40);
          tft.drawString("0.75  # Fluss der Pumpe 1 in ml pro sekunde",5,60);
          tft.drawString("0.9  # Fluss der Pumpe 2 in ml pro sekunde",5,80);
          tft.drawString("...",35,100);
          tft.drawString("0.82  # Fluss der Pumpe 11 in ml pro sekunde",5,120);
          break;

      case 4:
          tft.drawString("PINS",50,10);
          tft.drawString("HX711_dout 3  HX711_sck 5",5,40);
          tft.drawString("Pumppins",5,60);
          tft.drawString("11 13 15 19 21 23 27 37 16 18 7",5,80);
          break;
      case 5:
          break;
      case 6:
          break;
      case 7:
          break;
      case 8:
          break;
      case 9:
          break;
  }

};