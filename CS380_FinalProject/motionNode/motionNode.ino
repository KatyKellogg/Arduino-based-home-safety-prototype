//Authors: Will Keenan-Harte & Katy Kellogg
//Motion Sensor Node 
#include <SoftwareSerial.h>

enum mState {
  ARMED,
  DISARMED
};

String server;
SoftwareSerial XBee(0, 1);
const int sens3 = 2;
mState motion = DISARMED;
unsigned long timer = 0;
unsigned long cooldown = 0;

void setup() {
  Serial.begin(57600);
  pinMode(9,OUTPUT); 
  pinMode(sens3, INPUT_PULLUP);
}

void loop() {
  if(Serial.available() > 0) {
    server = Serial.readString();
    if (server == "arm\n") {motion = ARMED;}
    else if(server == "disarm\n") {motion = DISARMED;}
  }

  int mdetector = digitalRead(sens3);

  switch(motion) {
    case ARMED:
      //inti, first pass through switch
      if (timer == 0) {timer = millis();}
      //check for motion after 15 seconds has passed to warm up
      if (millis() - timer >= 15000) {
        if (mdetector == LOW && millis() - cooldown >= 5000) {
          cooldown = millis();
          Serial.println("m detected");
        }
      }
      break;

    case DISARMED:
      if (timer != 0) {timer = 0;}
      break;
  }
}