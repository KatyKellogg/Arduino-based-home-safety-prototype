//Authors: Will Keenan-Harte & Katy Kellogg
//Keypad Node 
#include <stdio.h>
/*
  Much of the debounce code comes from this tutorial
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
*/
int password[6] = {0,0,0,0,0,0};
int check[6] =    {-1,-1,-1,-1,-1,-1};
bool match = true;
int i = 0;

enum passwordStatus
{
  CHECK,
  RESET,
  IDLE
};

enum buttons
{
  BLUE,
  YELLOW,
  BLACK,
  GREEN
};

passwordStatus passStat = IDLE;

// initialize the blue button
const int bluePin = 3; 
const int blueLedPin = 8;   
int blueState;            // the current reading from the input pin
int lastBlueState = LOW;  // the previous reading from the input pin
//initialize the yellow button
const int yellowPin = 4; 
const int yellowLedPin = 9;   
int yellowState;            // the current reading from the input pin
int lastYellowState = LOW;  // the previous reading from the input pin
//initialize the black button
const int blackPin = 5; 
const int blackLedPin = 10;   
int blackState;            // the current reading from the input pin
int lastBlackState = LOW;  // the previous reading from the input pin
//initialize the green button
const int greenPin = 6; 
const int greenLedPin = 11;   
int greenState;            // the current reading from the input pin
int lastGreenState = LOW;  // the previous reading from the input pin
unsigned long lastBlueDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastYellowDebounceTime = 0;
unsigned long lastBlackDebounceTime = 0;
unsigned long lastGreenDebounceTime = 0;
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int blueReading;
int yellowReading;
int blackReading;
int greenReading;



void setup() {
  Serial.begin(57600);
  pinMode(bluePin, INPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(yellowPin, INPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(blackPin, INPUT);
  pinMode(blackLedPin, OUTPUT);
  pinMode(greenPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {

    switch(passStat)
    {
      case IDLE:
        digitalWrite(blueLedPin, LOW);
        digitalWrite(blackLedPin, LOW);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(greenLedPin, LOW);

        if(Serial.available() != 0)
        {
          String input = Serial.readString();
          if(input == "r\n")
          {
            passStat = RESET;
            break;
          }
          else if(input == "c\n")
          {
            passStat = CHECK;
            break;
          }
        }
        break;
      case CHECK:
        /*if(password[0] == -1)
        {
          passStat = RESET;
          Serial.println("RESETING PASSWORD");
          break;
        }*/
        // read the state of the switch into a local variable:
        blueReading = digitalRead(bluePin);
        yellowReading = digitalRead(yellowPin);
        blackReading = digitalRead(blackPin);
        greenReading = digitalRead(greenPin);

        // check to see if you just pressed the button
        // (i.e. the input went from LOW to HIGH), and you've waited long enough
        // since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        //**********BLUE****************
        if (blueReading != lastBlueState) {
          // reset the debouncing timer
          lastBlueDebounceTime = millis();
        }

        if ((millis() - lastBlueDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (blueReading != blueState) {
            blueState = blueReading;
            // only toggle the LED if the new button state is HIGH
            if (blueState == HIGH) {
              digitalWrite(blueLedPin, HIGH);
              if(i < 6)
              {
                check[i] = BLUE;
                i++;
              }

            }
            else
            {
              digitalWrite(blueLedPin, LOW);
            }
          }

        }

        //**********YELLOW****************
        if (yellowReading != lastYellowState) {
          // reset the debouncing timer
          lastYellowDebounceTime = millis();
        }

        if ((millis() - lastYellowDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (yellowReading != yellowState) {
            yellowState = yellowReading;
            // only toggle the LED if the new button state is HIGH
            if (yellowState == HIGH) {
              digitalWrite(yellowLedPin, HIGH);
              if(i < 6)
              {
                check[i] = YELLOW;
                i++;
              }

            }
            else
            {
              digitalWrite(yellowLedPin, LOW);
            }
          }
        }

        //**********BLACK****************
        if (blackReading != lastBlackState) {
          // reset the debouncing timer
          lastBlackDebounceTime = millis();
        }

        if ((millis() - lastBlackDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (blackReading != blackState) {
            blackState = blackReading;
            // only toggle the LED if the new button state is HIGH
            if (blackState == HIGH) {
              digitalWrite(blackLedPin, HIGH);
              if(i < 6)
              {
                check[i] = BLACK;
                i++;
              }

            }
            else
            {
              digitalWrite(blackLedPin, LOW);
            }
          }

        }

        //**********GREEN****************
        if (greenReading != lastGreenState) {
          // reset the debouncing timer
          lastGreenDebounceTime = millis();
        }

        if ((millis() - lastGreenDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (greenReading != greenState) {
            greenState = greenReading;
            // only toggle the LED if the new button state is HIGH
            if (greenState == HIGH) {
              digitalWrite(greenLedPin, HIGH);
              if(i < 6)
              {
                check[i] = GREEN;
                i++;
              }

            }
            else
            {
              digitalWrite(greenLedPin, LOW);
            }
          }

        }


        // save the reading. Next time through the loop, it'll be the lastButtonState:
        lastBlueState = blueReading;
        lastYellowState = yellowReading;
        lastBlackState = blackReading;
        lastGreenState = greenReading;

        if(i == 6)
        {
          for(int j = 0; j < 6; j ++)
          {
            if(password[j] != check[j])
            {
              Serial.println("k Incorrect!");
              i = 0;
              match = false;
              break;
            }
          }
          if(match == true)
          {
            Serial.println("k Correct!");
            i = 0;
          }
          match = true;
          passStat = IDLE;


        }
        break;
      
      case RESET:
        // read the state of the switch into a local variable:
        blueReading = digitalRead(bluePin);
        yellowReading = digitalRead(yellowPin);
        blackReading = digitalRead(blackPin);
        greenReading = digitalRead(greenPin);

        // check to see if you just pressed the button
        // (i.e. the input went from LOW to HIGH), and you've waited long enough
        // since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        //**********BLUE****************
        if (blueReading != lastBlueState) {
          // reset the debouncing timer
          lastBlueDebounceTime = millis();
        }

        if ((millis() - lastBlueDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (blueReading != blueState) {
            blueState = blueReading;
            // only toggle the LED if the new button state is HIGH
            if (blueState == HIGH) {
              digitalWrite(blueLedPin, HIGH);
              if(i < 6)
              {
                password[i] = BLUE;
                i++;
              }

            }
            else
            {
              digitalWrite(blueLedPin, LOW);
            }
          }

        }

        //**********YELLOW****************
        if (yellowReading != lastYellowState) {
          // reset the debouncing timer
          lastYellowDebounceTime = millis();
        }

        if ((millis() - lastYellowDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (yellowReading != yellowState) {
            yellowState = yellowReading;
            // only toggle the LED if the new button state is HIGH
            if (yellowState == HIGH) {
              digitalWrite(yellowLedPin, HIGH);
              if(i < 6)
              {
                password[i] = YELLOW;
                i++;
              }

            }
            else
            {
              digitalWrite(yellowLedPin, LOW);
            }
          }
        }

        //**********BLACK****************
        if (blackReading != lastBlackState) {
          // reset the debouncing timer
          lastBlackDebounceTime = millis();
        }

        if ((millis() - lastBlackDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (blackReading != blackState) {
            blackState = blackReading;
            // only toggle the LED if the new button state is HIGH
            if (blackState == HIGH) {
              digitalWrite(blackLedPin, HIGH);
              if(i < 6)
              {
                password[i] = BLACK;
                i++;
              }

            }
            else
            {
              digitalWrite(blackLedPin, LOW);
            }
          }

        }

        //**********GREEN****************
        if (greenReading != lastGreenState) {
          // reset the debouncing timer
          lastGreenDebounceTime = millis();
        }

        if ((millis() - lastGreenDebounceTime) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (greenReading != greenState) {
            greenState = greenReading;
            // only toggle the LED if the new button state is HIGH
            if (greenState == HIGH) {
              digitalWrite(greenLedPin, HIGH);
              if(i < 6)
              {
                password[i] = GREEN;
                i++;
              }

            }
            else
            {
              digitalWrite(greenLedPin, LOW);
            }
          }

        }

        // save the reading. Next time through the loop, it'll be the lastButtonState:
        lastBlueState = blueReading;
        lastYellowState = yellowReading;
        lastBlackState = blackReading;
        lastGreenState = greenReading;

        if(i >= 6)
        {
          Serial.println("k New Password Set");
          i = 0;
          passStat = IDLE;
        }
        break;
    }



}
