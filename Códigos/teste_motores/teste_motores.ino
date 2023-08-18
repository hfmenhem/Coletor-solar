#include <Wire.h>
#include <AccelStepper.h>

#define FULLSTEP 4
AccelStepper myStepperA(FULLSTEP, 11, 9, 10, 8);
AccelStepper myStepperh(FULLSTEP, 7, 5, 6, 4);
  

void setup() {

 Serial.begin(9600); 
  // put your setup code here, to run once:
  myStepperA.setMaxSpeed(1000.0);
  myStepperA.setSpeed(1000);
  myStepperA.setAcceleration(50);
  myStepperh.setMaxSpeed(1000.0);
  myStepperh.setSpeed(1000);
  myStepperh.setAcceleration(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() == 0) {
      Serial.print("0A+");
      Serial.print("\t");
      Serial.print("1A-");
      Serial.print("\t");
      Serial.print("2h+");
      Serial.print("\t");
      Serial.println("3h-");
      }
  int valor = Serial.parseInt(SKIP_ALL);
  Serial.readString();  // "Lê" e limpa o buffer serial
  if(valor==0){
    myStepperA.move(2038);
  }
  if(valor==1){
    myStepperA.move(-2038);
  }
  if(valor==2){
    myStepperh.move(2038);
  }
  if(valor==3){
    myStepperh.move(-2038);
  }
  if(valor==4){
    int i = 0;
    while (i<245){
      myStepperA.move(100);
      myStepperA.runToPosition();
      i++;
    Serial.println(i);
    }
  }
  if(valor==5){
    int i = 0;
    while (i<61){
      myStepperh.move(-100);
      myStepperh.runToPosition();
      i++;
    Serial.println(i);
    }
  }

  myStepperh.runToPosition();
  myStepperA.runToPosition();
}
