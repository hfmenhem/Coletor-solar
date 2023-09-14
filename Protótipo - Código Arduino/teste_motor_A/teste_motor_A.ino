#include <Wire.h>
#include <AccelStepper.h>

#define FULLSTEP 4
AccelStepper myStepperA(FULLSTEP, 8, 10, 9, 11);
AccelStepper myStepperh(FULLSTEP, 4, 6, 5, 7);
  
int rot;
#define limA 13

void setup() {

 Serial.begin(9600); 
  // put your setup code here, to run once:
  myStepperA.setMaxSpeed(200.0);
  myStepperA.setSpeed(200);
  myStepperA.setAcceleration(200);
  myStepperh.setMaxSpeed(200.0);
  myStepperh.setSpeed(200);
  myStepperh.setAcceleration(200);
  //rot = 2038*12/360;
  rot = 2038*12/345;
}

void loop() {
  // put your main code here, to run repeatedly:
  while (digitalRead(limA) != 1){
    myStepperA.move(rot*360);
    while (digitalRead(limA) != 1 && myStepperA.distanceToGo() != 0){
      myStepperA.run();
    }
  }
  myStepperA.stop();
  delay(5000);
  myStepperA.move (-rot*360);
  myStepperA.runToPosition();
  delay(5000);
}
