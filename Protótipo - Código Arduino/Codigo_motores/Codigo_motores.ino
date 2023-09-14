#include <math.h>
#include <Wire.h>
#include <RTClib.h>
#include <AccelStepper.h>

RTC_DS1307 RTC;

#define FULLSTEP 4
//AccelStepper myStepperA(FULLSTEP, 11, 9, 10, 8);
//AccelStepper myStepperh(FULLSTEP, 7, 5, 6, 4);
AccelStepper myStepperA(FULLSTEP, 8, 10, 9, 11);
AccelStepper myStepperh(FULLSTEP, 4, 6, 5, 7);


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

float lon;
float lat;

int tDia;
float tRad;

float h;
float A;

float h2;
float a2;

int h0;
int a0;

int horaN;

void setup() {
  //para calcular valores de azimute e altura
  lon = (45 - 44.896) * 0.017453;
  lat = (23.124) * 0.017453;

  Serial.begin(9600);
  Serial.println("");
  Serial.println("--------------");

  //relógio
  Wire.begin();
  if (! RTC.begin()) {
    Serial.println("não foi possível achar o RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! RTC.isrunning()) {
    Serial.println("RTC não está rodando, acerte a hora");
    acertarHora();
  }else{
    Serial.println("Hora:");
    DateTime time = RTC.now();
    Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

    Serial.println("acertar hora? digite qualquer coisa");
    delay(10000);
    if(Serial.available() == 0){
      Serial.println("não :(");
    }else{
      Serial.println("sim :)");
      acertarHora();
    }
  }
  //motores de passo
  myStepperA.setMaxSpeed(200.0);
  myStepperA.setSpeed(200);
  myStepperA.setAcceleration(200);
  myStepperh.setMaxSpeed(200.0);
  myStepperh.setSpeed(200);
  myStepperh.setAcceleration(200);
  rot = 2038*12/360;

  pinMode(limh, INPUT);
  pinMode(limA, INPUT);
  
  zero();
  
  delay(10000);
}

void loop() {
  
  DateTime now = RTC.now(); 

  ano = now.year();
  mes = now.month();
  dia = now.day();
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();
  if (hora != horaN){
    horaN = hora;
    zero();
  }

  //Salva valores anteriores
  
  //calcula A e h
  posicao();

  Serial.print("A2:");
  Serial.print(a2);
  Serial.print("\t");
  Serial.print("A:");
  Serial.println(A);
  
  Serial.print("h2:");
  Serial.print(h2);
  Serial.print("\t");
  Serial.print("h:");
  Serial.println(h);

  //aponta o motor para o local certo
  if(((h-h2)>=1)||((h-h2)<=-1)){
    myStepperh.moveTo((1*h*rot) + h0 -(90*rot));
    myStepperh.runToPosition();  
    h2=h;
  }
  if(((A-a2)>=1)||((A-a2)<=-1)){
    myStepperA.moveTo((-1*A*rot) + a0);
    myStepperA.runToPosition();
    a2=A;  
  }
  delay(60000UL);
}


void posicao() {
  // tempo
  float diaTemp = dia + 30.42 * (mes - 11) + 365.25 * (ano - 2022);
  tDia = round(diaTemp);
  tRad = (segundo / 13751.0) + (minuto / 229.18) + (hora / 3.8197);

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
    A = (2*3.1415)- acos(cosA);
  }
  if(h<0){
    h=0;
  }

  //converte para graus
  h = h * 57.29578;
  A = A * 57.29578;
}

void acertarHora(){
  Serial.readString();
  Serial.print("ano: ");
  while (Serial.available() == 0) {}
  ano = Serial.parseInt();
  Serial.readString();
  Serial.println(ano);
  delay(1000);
  
  Serial.print("mês: ");
  while (Serial.available() == 0) {}
  mes = Serial.parseInt();
  Serial.readString();
  Serial.println(mes);
  delay(1000);
  
  Serial.print("dia: ");
  while (Serial.available() == 0) {}
  dia = Serial.parseInt();
  Serial.readString();
  Serial.println(dia);
  delay(1000);
  
  Serial.print("hora: ");
  while (Serial.available() == 0) {}
  hora = Serial.parseInt();
  Serial.readString();
  Serial.println(hora);
  delay(1000);
  
  Serial.print("minuto: ");
  while (Serial.available() == 0) {}
  minuto = Serial.parseInt();
  Serial.readString();
  Serial.println(minuto);
  delay(1000);
  
  Serial.print("segundo: ");
  while (Serial.available() == 0) {}
  segundo = Serial.parseInt();
  Serial.readString();
  Serial.println(segundo);
      
  RTC.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
  DateTime now = RTC.now(); 
  ano = now.year();
  mes = now.month();
  dia = now.day();
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();
  DateTime time = RTC.now();
  Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

} 

void zero(){
  while (digitalRead(limh) != 1){
    myStepperh.move(rot*90);
    while (digitalRead(limh) != 1 && myStepperh.distanceToGo() != 0){
      myStepperh.run();
    }
  }
  myStepperh.stop();
  h0 = myStepperh.currentPosition();
  myStepperh.move(h0);


  while (digitalRead(limA) != 1){
    myStepperA.move(rot*360);
    while (digitalRead(limA) != 1 && myStepperA.distanceToGo() != 0){
      myStepperA.run();
    }
  }
  myStepperA.stop();
  a0 = myStepperA.currentPosition();
  myStepperA.move(a0);

  a2=0;
  h2=90;
}
