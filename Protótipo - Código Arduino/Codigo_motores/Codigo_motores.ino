#include <math.h>
#include <Wire.h>
#include <RTClib.h>
#include <AccelStepper.h>

RTC_DS1307 RTC;

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

float lon;
float lat;

int tDia;
float tRad;

float h;
float A;

int h0;
int a0;

int horaN;

void setup() {
  //para calcular valores de azimute e altura
  lon = (45 - 46.65) * 0.017453;
  lat = (23.568) * 0.017453;

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
  myStepperA.setMaxSpeed(400.0);
	myStepperA.setSpeed(400);
  myStepperA.setAcceleration(50);
  myStepperh.setMaxSpeed(200.0);
	myStepperh.setSpeed(200);
  myStepperh.setAcceleration(50);
  rot = 2038*12/360;

  while (digitalRead(limh) != 1){
    myStepperh.move(100);
    myStepperh.run();
  }
  myStepperh.stop();
  h0 = myStepperh.currentPosition();

  while (digitalRead(limA) != 1){
    myStepperA.move(200);
    myStepperA.run();
  }
  myStepperA.stop();
  a0 = myStepperA.currentPosition();

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
    while (digitalRead(limh) != 1){
      myStepperh.move(100);
      myStepperh.run();
    }
    myStepperh.stop();
    h0 = myStepperh.currentPosition();

    while (digitalRead(limA) != 1){
      myStepperA.move(100);
      myStepperA.run();
    }
    myStepperA.stop();
    a0 = myStepperA.currentPosition();
  }
  
  //calcula A e h
  posicao();

  Serial.print("A:");
  Serial.println(A);
  Serial.print("h:");
  Serial.println(h);

  //aponta o motor para o local certo
  myStepperh.moveTo((1*h*rot) + h0 -(90*rot));
  myStepperh.runToPosition();
  myStepperA.moveTo((-1*A*rot) + a0);
  myStepperA.runToPosition();
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
