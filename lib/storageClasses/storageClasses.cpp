#include <storageClasses.h>
bool Flasche::parseFlasche(String filename){
        bool flaskOk = false;
        File myFile = SD.open("flasche"+filename+".txt"); // open the file for reading
        if (myFile) { 
          overlayWrite("flasche"+filename+".txt");
          int lineNr = 0;
          while (myFile.available()) { //execute while file is available
            String line = myFile.readStringUntil('\n');  // \n character is discarded from buffer
            if (line.indexOf("#") > 0){
              line = line.substring(0,line.indexOf("#"));
            }
            lineNr = lineNr+1;
            tft.print(lineNr);overlayWrite(": "+line);
            if (lineNr==1){
              for (int i = 0; i < min(15,line.length()); i++){
                name[i] = line[i];
              }
              nameLength = line.length();
              flaskOk = true;
              tft.print("name: ");overlayWrite(String(name).substring(0,nameLength)+"#end");
            }

            if (lineNr==2){
              fassungsVermoegen_ml = line.toFloat();
            }

            if (lineNr==3){
              fluessigkeit_schaetzung = line.toFloat();
            }
            
          }
          myFile.close(); //close file
        }
        else {
          overlayWrite("missing:");
          overlayWrite("flasche"+filename+".txt");
          delay(200);
          if (filename == "1"){
            loadError = true;
          }
        }
        return flaskOk;
    };





Flasche flaschen[12] = {};


bool Rezept::parseRezept(String filename){
        File myFile = SD.open("rezept"+filename+".txt"); // open the file for reading
        bool recipeOK = false;
        if (myFile) { 
          overlayWrite("found rezept"+filename+".txt");
          int schrittNr = 0;
          int lineNr = 0;
          schritt[schrittNr][11] = 0;
          while (myFile.available()) { //execute while file is available
            String line = myFile.readStringUntil('\n');  // \n character is discarded from buffer
            if (line.indexOf("#") > 0){
              line = line.substring(0,line.indexOf("#"));
            }
            lineNr = lineNr+1;
            tft.print(lineNr);overlayWrite(": "+line);
            
            if (lineNr==1){
              overlayWrite("expecting: name");
              for (int i = 0; i < min(20,line.length()); i++){
                name[i] = line[i];
              }
              nameLength = line.length();
            }

            if (lineNr==2){
              overlayWrite("expecting: short desc");
              for (int i = 0; i < min(20,line.length()); i++){
                shortText1[i] = line[i];
              }
              shortTextLength1 = line.length();
            }

            if (lineNr==3){
              overlayWrite("expecting: short desc2");
              for (int i = 0; i < min(20,line.length()); i++){
                shortText2[i] = line[i];
              }
              shortTextLength2 = line.length();
            }

            if (lineNr==4){
              overlayWrite("expecting: short desc3");
              for (int i = 0; i < min(20,line.length()); i++){
                shortText3[i] = line[i];
              }
              shortTextLength3 = line.length();
            }

            if (lineNr==5){
              overlayWrite("expecting: end desc1");
              for (int i = 0; i < min(20,line.length()); i++){
                endText1[i] = line[i];
              }
              endTextLength1 = line.length();
            }

            if (lineNr==6){
              overlayWrite("expecting: end desc2");
              for (int i = 0; i < min(20,line.length()); i++){
                endText2[i] = line[i];
              }
              endTextLength2 = line.length();
              overlayWrite("expecting: flaskname-ml");
              overlayWrite("expecting: ...");
              overlayWrite("--- for new Step");
              overlayWrite("(up to 3)");
            }

            if (lineNr > 6){
               if(line.startsWith("---")){
                  schrittNr++;
                  schritt[schrittNr][11] = 0;
                  overlayWrite("new recipeStep");
                  continue;
               }
              
              String zutat = line.substring(0,line.indexOf("-"));
              float value = line.substring(line.indexOf("-")+1).toFloat();
              //search for corresponding flask
              bool correctFlaskFound = false;
              for (int flaschenNr = 0; flaschenNr < 11; flaschenNr++){
                if (flaschen[flaschenNr].nameLength == zutat.length()){
                  bool ok = true;
                  for (int b = 0; b<zutat.length(); b++){
                    if (flaschen[flaschenNr].name[b] != zutat[b]){
                      ok = false;
                    }
                  }
                  if (ok){
                      recipeOK = true;
                      correctFlaskFound = true;
                      overlayWrite("zutat: "+String(flaschen[flaschenNr].name).substring(0,flaschen[flaschenNr].nameLength)+" "+String(value));
                      schritt[schrittNr][flaschenNr] = value;
                  }

                }
              }
              if (!correctFlaskFound){
                  schritt[schrittNr][11] = schritt[schrittNr][11] + value; //dummy for manual fillup
              }
            }
          }
          myFile.close(); //close file
        }
        else {
          overlayWrite("missing:");
          overlayWrite("rezept"+filename+".txt");
          delay(100);
          if (filename == "1"){
            loadError = true;
          }
        }
        return recipeOK;
    };




Rezept rezepte[35] = {};
