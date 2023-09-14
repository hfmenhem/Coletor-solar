#include <math.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SD.h>   //https://docs.arduino.cc/learn/programming/sd-guide
#include <SPI.h>

//para medir as placas
#define PWM 6
//LED indicativo da escrita no SD
#define LED 2

Adafruit_INA219 controle(0x40);
float TfonteC_V = 0;
float CorrenteC_mA = 0;
float PotenciaC_mW = 0;

Adafruit_INA219 experimental(0x41);
float TfonteE_V = 0;
float CorrenteE_mA = 0;
float PotenciaE_mW = 0;

void setup() {
  pinMode(LED, OUTPUT);
  //para medir as placas
  controle.begin();
  experimental.begin();

  TfonteC_V = controle.getBusVoltage_V() + (controle.getShuntVoltage_mV() / 1000);
  CorrenteC_mA = controle.getCurrent_mA();
  PotenciaC_mW = controle.getPower_mW();

  TfonteE_V = experimental.getBusVoltage_V() + (experimental.getShuntVoltage_mV() / 1000);
  CorrenteE_mA = experimental.getCurrent_mA();
  PotenciaE_mW = experimental.getPower_mW();

  //coisas para mostrar na serial
  Serial.begin(9600);
  Serial.println("");
  Serial.println("--------------");

  Serial.println("Teste dos sensores:");

  Serial.println("");
  Serial.print("Ic (mA)");
  Serial.print("\t");
  Serial.print("Uc (V)");
  Serial.print("\t");
  Serial.print("Pc (mW)");
  Serial.print("\t");
  Serial.print("Ie (mA)");
  Serial.print("\t");
  Serial.print("Ue (V)");
  Serial.print("\t");
  Serial.print("Pe (mW)");
  Serial.print("\t");
  Serial.println("T (ms)");
}

void loop() {
  //para medir as placas
  digitalWrite(LED, HIGH);
  for (int i = 0; i <= 255; i++) {
    analogWrite(PWM, i);
    TfonteC_V = controle.getBusVoltage_V() + (controle.getShuntVoltage_mV() / 1000);
    CorrenteC_mA = controle.getCurrent_mA();
    PotenciaC_mW = controle.getPower_mW();

    TfonteE_V = experimental.getBusVoltage_V() + (experimental.getShuntVoltage_mV() / 1000);
    CorrenteE_mA = experimental.getCurrent_mA();
    PotenciaE_mW = experimental.getPower_mW();

    Serial.print(CorrenteC_mA);
    Serial.print("\t");
    Serial.print(TfonteC_V);
    Serial.print("\t");
    Serial.print(PotenciaC_mW);
    Serial.print("\t");
    Serial.print(CorrenteE_mA);
    Serial.print("\t");
    Serial.print(TfonteE_V);
    Serial.print("\t");
    Serial.print(PotenciaE_mW);
    Serial.print("\t");
    Serial.print(millis());
    Serial.print("\t");
    Serial.println(i);
    delay(2);
  }
  Serial.println("");
  Serial.println("");
  digitalWrite(PWM, HIGH);
  digitalWrite(LED, LOW);

  delay (60000UL);
}
