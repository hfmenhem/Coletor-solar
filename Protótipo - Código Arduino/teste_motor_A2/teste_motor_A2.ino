#include <Wire.h>
#include <AccelStepper.h>

#define FULLSTEP 4
AccelStepper myStepperA(FULLSTEP, 8, 10, 9, 11);
  
int rot;
#define limA 13
int ang;
int a0;
int delta=20;
void setup() {
  pinMode(limA, INPUT);
  Serial.begin(9600); 
  // put your setup code here, to run once:
  myStepperA.setMaxSpeed(200.0);
  myStepperA.setSpeed(200);
  myStepperA.setAcceleration(200);

  rot = 2038*12/360;

  Serial.print("Angulo:");
  while(!Serial.available()){}
  ang = Serial.parseInt(SKIP_ALL);
    Serial.println(ang);
  while (digitalRead(limA) != 1){
    myStepperA.move(rot*360);
    while (digitalRead(limA) != 1 && myStepperA.distanceToGo() != 0){
      myStepperA.run();
    }
  }
  myStepperA.stop();
  a0 = myStepperA.currentPosition();
}

void loop() {
  myStepperA.moveTo(a0-1*rot*(ang-delta));
  myStepperA.runToPosition();
  delay(1000);
  myStepperA.moveTo(a0-1*rot*(ang+delta));
  myStepperA.runToPosition();
  delay(1000);
}
