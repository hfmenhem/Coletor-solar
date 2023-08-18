#include <math.h>
#include <Wire.h>
#include <AccelStepper.h>


#define FULLSTEP 4
AccelStepper myStepperA(FULLSTEP, 11, 9, 10, 8);
AccelStepper myStepperh(FULLSTEP, 7, 5, 6, 4);

float rot;

#define limh 12
#define limA 13

//para calcular valores de azimute e altura
int ano;
int mes;
int dia;
int hora;
int minuto;
int segundo;

int tDia;
float tRad;

float h;
float A;

int h0;
int a0;

void setup() { 
  Serial.begin(9600);
  Serial.println("");
  Serial.println("--------------");
  
  //motores de passo
  myStepperA.setMaxSpeed(1000.0);
	myStepperA.setSpeed(1000);
  myStepperA.setAcceleration(50);
  myStepperh.setMaxSpeed(1000.0);
	myStepperh.setSpeed(1000);
  myStepperh.setAcceleration(50);
  rot = 2038*3/360;
  
  Serial.println("Iniciando...");
  calibragem();
  delay(1000); 
}

void loop() {
  
  if (Serial.available() != 0) { //se receber alguma coisa da porta serial
    int func = Serial.parseInt();
    switch (func){
      case 10:
        Serial.println("Calibragem...");
        calibragem();
        break;
      case 1:
        Serial.println("Giro...");
        giro();
        break;
      case 2:
        Serial.println("Posicao...");
        posicao();
        break;
       case 3:
        Serial.println("Circumpolar...");
        circumpolar();
        break;
      case 4:
        Serial.println("mostrar...");
        mostrar();
        break;
    }
  }
  
  //giro();
  //t0 = millis();
  //posicao();
  
  //Serial.println(A);
  //aponta o motor para o local certo
  //myStepperA.moveTo((-1*A*rot) + a0);
  //myStepperA.runToPosition();
  //myStepperh.moveTo((-1*h*rot) + h0);
  //myStepperh.runToPosition();
  
  //delay(60000UL);
}

void giro(){
  myStepperA.setMaxSpeed(1000.0);
  myStepperA.setSpeed(1000);
  myStepperA.setAcceleration(50);
  myStepperh.setMaxSpeed(1000.0);
  myStepperh.setSpeed(1000);
  myStepperh.setAcceleration(50);


  myStepperh.moveTo(h0);
  myStepperh.runToPosition();  
  myStepperh.moveTo(h0);
  myStepperh.runToPosition();  
  
  myStepperh.moveTo((-1*30*rot) + h0);
  myStepperh.runToPosition();  
  
  myStepperA.moveTo((-1*180*rot) + a0);
  myStepperA.runToPosition();


  myStepperA.moveTo((-1*0*rot) + a0);
  myStepperA.runToPosition();

  myStepperh.moveTo((-1*150*rot) + h0);
  myStepperh.runToPosition();

  myStepperA.moveTo((-1*180*rot) + a0);
  myStepperA.runToPosition();
  
}

void posicao() { 
    float lon = (45 - 46.84) * 0.017453;
    float lat = (23.549) * 0.017453;
    int t =0; 
    //Posiciona para Leste
    h = 0;
    A = 90;
    
    myStepperh.moveTo((-1*h*rot) + h0);
    myStepperA.moveTo((-1*A*rot) + a0);

    while ((myStepperA.distanceToGo() != 0) && (myStepperh.distanceToGo() != 0)){
      myStepperA.run();
      myStepperh.run();
    }
    while (myStepperA.distanceToGo() != 0){
      myStepperA.run();;
    }
    while (myStepperh.distanceToGo() != 0){
      myStepperh.run();;
    }
    
    int t0 = millis();
  while(h<180){  
    t = (millis()-t0)/2;
    // tempo
    float diaTemp = 21 + 30.42 * (3 - 11) + 365.25 * (2023 - 2022);
    tDia = round(diaTemp);
    tRad = (t / 13751.0) + (0 / 229.18) + (6 / 3.8197);
  
    // equação do tempo e declinação
    float L = 3.838 + 0.0172 * tDia;
    float M = 5.175 + 0.0172 * tDia;
    float eqT = (0.04302 * sin(2 * L)) - (0.0334 * sin(M));
    float dec = asin(0.39773 * sin(L));
  
    //ângulo horário
    float AH = lon + eqT + tRad - 3.1415; //em radiano
  
    //altura
    h = asin((cos(lat) * cos(dec) * cos(AH)) - (sin(lat) * sin(dec)));
  
    //azimute
    float sinA = -1 * sin(AH) * cos(dec) / cos(h);
    float cosA = ((sin(dec) * cos(lat)) + (cos(dec) * sin(lat) * cos(AH))) / cos(h);
    
    if (sinA >= 0) {
      A = acos(cosA);
    } else {
      A = acos(-1*cosA);
      h = 3.141593 - h;
    }
  
    //converte para graus
    h = h * 57.29578;
    A = A * 57.29578;
    
    myStepperh.moveTo((-1*h*rot) + h0);
    myStepperA.moveTo((-1*A*rot) + a0);

    while ((myStepperA.distanceToGo() != 0) && (myStepperh.distanceToGo() != 0)){
      myStepperA.run();
      myStepperh.run();
    }
    while (myStepperA.distanceToGo() != 0){
      myStepperA.run();;
    }
    while (myStepperh.distanceToGo() != 0){
      myStepperh.run();;
    }
  }
}

void circumpolar() { 
  float lat = 40 * 0.017453;  
  int t =0;
  int t0 = millis();
  tRad = 0;
  
  while(tRad < 6.2832){
    t = (millis()-t0)*2;
    // tempo
    tRad = (t / 13751.0);
    float dec = -70 * 0.017453;; //70 graus em radianos
  
    //ângulo horário
    float AH = tRad - 3.1415; //em radiano
    
    //altura
    h = asin((cos(lat) * cos(dec) * cos(AH)) - (sin(lat) * sin(dec)));
  
    //azimute
    float sinA = -1 * sin(AH) * cos(dec) / cos(h);
    float cosA = ((sin(dec) * cos(lat)) + (cos(dec) * sin(lat) * cos(AH))) / cos(h);

    float C = sinA;
    sinA = cosA;
    cosA = C;
     
    if (sinA >= 0) {
      A = acos(cosA);
      h = 3.141593 - h;
    } else {
      A = acos(-1*cosA);
      
    }
  
    //converte para graus
    h = h * 57.29578;
    A = A * 57.29578;
  
    myStepperh.moveTo((-1*h*rot) + h0);
    myStepperA.moveTo((-1*A*rot) + a0);

    while ((myStepperA.distanceToGo() != 0) && (myStepperh.distanceToGo() != 0)){
      myStepperA.run();
      myStepperh.run();
    }
    while (myStepperA.distanceToGo() != 0){
      myStepperA.run();;
    }
    while (myStepperh.distanceToGo() != 0){
      myStepperh.run();;
    }
    
  }
}

void calibragem(){
  while (digitalRead(limh) != 1){
    myStepperh.move(100);
    myStepperh.run();
  }
  myStepperh.stop();
  h0 = myStepperh.currentPosition();
  myStepperh.moveTo(h0);
  myStepperh.runToPosition();
  
  while (digitalRead(limA) != 1){
    myStepperA.move(100);
    myStepperA.run();
  }
  myStepperA.stop();
  a0 = myStepperA.currentPosition();

  myStepperA.moveTo(a0);
  myStepperA.runToPosition();
}

void mostrar(){
  h = 150;
  A = 0;
  myStepperh.moveTo((-1*h*rot) + h0);
  myStepperA.moveTo((-1*A*rot) + a0);
    
  myStepperA.runToPosition();
  myStepperh.runToPosition();
  
}
