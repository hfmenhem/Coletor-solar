#include <math.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SD.h>   //https://docs.arduino.cc/learn/programming/sd-guide
#include <SPI.h>

//para salvar os dados
File myFile;
#define pinSS 10 // não pode mudar o nome de pinSS, isso é um nome específico da biblioteca!
String nomeAr;

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
  Serial.println(millis());


  nomeAr = "teste.txt";

  Serial.println("");
  Serial.print("nome do arquivo: ");
  Serial.println(nomeAr);

  if (SD.begin(10)) { // Inicializa o SD Card
    Serial.println("SD Card pronto para uso."); // Imprime na tela
  } else {
    Serial.println("Falha na inicialização do SD Card.");
    return;
  }

  int j = 0;
  while (!myFile || j < 5){
    myFile = SD.open(nomeAr, FILE_WRITE); // Cria / Abre arquivo .txt
    j ++;
  } 
  if (myFile) {
    myFile.println("");
    myFile.print("Ic (mA)");
    myFile.print("\t");
    myFile.print("Uc (V)");
    myFile.print("\t");
    myFile.print("Pc (mW)");
    myFile.print("\t");
    myFile.print("Ie (mA)");
    myFile.print("\t");
    myFile.print("Ue (V)");
    myFile.print("\t");
    myFile.print("Pe (mW)");
    myFile.print("\t");
    myFile.println("T (ms)");
     
    myFile.close();
  }
  else {     // Se o Arquivo não abrir
    Serial.println("Erro ao Abrir Arquivo .txt"); // Imprime na tela
  }
}

void loop() {
  //para medir as placas
  digitalWrite(LED, HIGH);
  myFile = SD.open(nomeAr, FILE_WRITE);
  if (myFile) { // Se o Arquivo abrir:
    analogWrite(PWM, 0);
    TfonteC_V = controle.getBusVoltage_V() + (controle.getShuntVoltage_mV() / 1000);
    CorrenteC_mA = controle.getCurrent_mA();
    PotenciaC_mW = controle.getPower_mW();

    TfonteE_V = experimental.getBusVoltage_V() + (experimental.getShuntVoltage_mV() / 1000);
    CorrenteE_mA = experimental.getCurrent_mA();
    PotenciaE_mW = experimental.getPower_mW();

    myFile.print(CorrenteC_mA);
    myFile.print("\t");
    myFile.print(TfonteC_V);
    myFile.print("\t");
    myFile.print(PotenciaC_mW);
    myFile.print("\t");
    myFile.print(CorrenteE_mA);
    myFile.print("\t");
    myFile.print(TfonteE_V);
    myFile.print("\t");
    myFile.print(PotenciaE_mW);
    myFile.print("\t");
    myFile.println(millis());

    myFile.println("");
    myFile.println("");
    myFile.close();
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
    Serial.println(millis());
  
  }
  else {     // Se o Arquivo não abrir
    Serial.println("Erro ao Escrever no Arquivo .txt"); // Imprime na tela
  }
  digitalWrite(PWM, LOW);
  digitalWrite(LED, LOW);
}
