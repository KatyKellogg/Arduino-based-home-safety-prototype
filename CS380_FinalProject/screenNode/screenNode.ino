//Authors: Will Keenan-Harte & Katy Kellogg
//Screen and Environment Sensor Node 
/*LCD-Hello_world.ino
Large chunk of this code comes from https://learn.sparkfun.com/tutorials/basic-character-lcd-hookup-guide
written by Sparkfun Electronics, Modified by: Ho Yun "Bobby" Chan
*/

#include <LiquidCrystal.h>
#include <Wire.h>
#include "SparkFunBME280.h"
#include "SparkFunCCS811.h"

//initialize the library by associating any needed LCD interface pin
//with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define CCS811_ADDR 0x5b
unsigned long lastDisplay = 0;
unsigned long lightDelay = 0;
unsigned long disDelay = 5000;
int hot = 105;
int cold = 0;
bool lightStat = false;

enum mState {
  ARMED,
  DISARMED
};

mState sensors = DISARMED;

BME280 sens1;
CCS811 sens2(CCS811_ADDR);

void setup() {
  //set up the LCD's number of columns and rows:
  Serial.begin(57600);
  Wire.begin();

  lcd.begin(16, 2);
  
  if (sens1.beginI2C() == false) {Serial.println("t BME280 error, failed to start");} 
  else if (sens2.begin() == false) {Serial.println("e CCS811 error, failed to start");}
  pinMode(7, OUTPUT);
  //Print a message to the LCD.
}

void loop() {
  //Update message text from server if availible
  if(Serial.available() > 0) {
    String server = Serial.readString();
    if (server == "arm\n") {sensors = ARMED;}
    else if(server == "disarm\n") {sensors = DISARMED;}
    else if(server.charAt(0) == 't') {
      String tempStr = server.substring(2);
      int dashPos = tempStr.indexOf(",");
      String coldStr = server.substring(2, dashPos+2);
      String hotStr = server.substring(dashPos + 3);
      cold = coldStr.toInt();
      hot = hotStr.toInt();
      Serial.println("t temps updated");
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print(server);
    }
  }

  if(millis() - lastDisplay >= disDelay && sensors == ARMED) {
    lastDisplay = millis();
    sens2.readAlgorithmResults();
    String tempF = String((int)sens1.readTempF());
    String co2 = String(sens2.getCO2());

    String temp = "Temp:" + tempF + "F CO2:"  + co2 + "ppb";
    ePrint0(temp);
  }

  switch (sensors) {
    case ARMED:
      if((int)sens1.readTempF() > hot || (int)sens1.readTempF() < cold) {
        
        if(millis() - lightDelay > 1000) {
          lightDelay = millis();
          if(lightStat && sensors == ARMED) {
            lightStat = false;
            digitalWrite(7, LOW);
          } else if (lightStat == false && sensors == ARMED) {
            lightStat = true;
            digitalWrite(7, HIGH);
          }
        }
      }
      else
      {
        digitalWrite(7, LOW);
      }
      if(Serial.available() > 0)
      {
        String server = Serial.readString();
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(server);
      }
      break;
    case DISARMED:
      lightStat = false; 
      digitalWrite(7, LOW);
  }
}
       


/*
  sens2.readAlgorithmResults();
  String tempF = String(sens1.readTempF());
  String co2 = String(sens2.getCO2());
  String tVOC = String(sens2.getTVOC());
  String line0 = "Temp:" + tempF + "F";
  String line1 = "CO2:" + co2 + " tVOC:" + tVOC;
  Serial.println(sens1.readTempF());
  Serial.println(sens2.getCO2());
  ePrint0(line0);
  ePrint1(line1);
  delay(5000);
*/
void ePrint0(String input) {
  //set the cursor to column 0, line 1
  //(note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(input);
}

void ePrint1(String input) {
  lcd.setCursor(0, 1);
  lcd.print(input);
}