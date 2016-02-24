#include <Wire.h>
#include <SPI.h>
#include "RCSwitch.h"
#include "MFRC522.h"
#include "Ultrasonic.h"

#define DESLOCAMENTO_ID 24
#define DESLOCAMENTO_RFID 16
#define DESLOCAMENTO_STATUSVAGA 8


#define RF_MIN 20
#define RF_MAX 30


#define SS_PIN 10
#define RST_PIN 9

#define ECHO_PIN 5
#define TRIGGER_PIN 3

// instancias
RCSwitch emissor  = RCSwitch();
RCSwitch receptor = RCSwitch();
MFRC522 rfid(SS_PIN, RST_PIN);
Ultrasonic ultrasonico(TRIGGER_PIN, ECHO_PIN);

struct InfoRF {
  int id; 
  int rfid;
  int statusVaga;
  int distancia;
} infoRF;

void setup() {
  emissor.enableTransmit(4);
  receptor.enableReceive(0);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
 long info = 0;
  info = lerSensores();
  emitir(info);
  delay(500);
  
  info = receber();
 
  
  if(info != -1) {
    if(IDValido(info)) {
     
      enviarParaUSB(info);
    } 
  }
}

long lerSensores() {
  long uid = 0;
  long statusVaga = 0;
  long distancia = pegarDistancia();
  if(distancia < 10) {
    statusVaga = 1;
    uid = pegarUID();
  }else{
    uid = 0;
  }
  
  
  long id = RF_MIN;
  long info = id << DESLOCAMENTO_ID;
  info = info | (uid << DESLOCAMENTO_RFID);
  info = info | (statusVaga << DESLOCAMENTO_STATUSVAGA);
  info = info | distancia;
  return info;
  
}

int IDValido(long info) {
  int valido = 0;
  infoRF.id = info >> DESLOCAMENTO_ID;
  if(( infoRF.id >= RF_MIN) && (infoRF.id <= RF_MAX)) {
    valido = 1;
  }
  return valido;
}

void emitir(long info) {
  emissor.send(info, 32);
}

long receber() {
  long info = -1;
  if(receptor.available()) {
    info = receptor.getReceivedValue();
    receptor.resetAvailable();
  }
  return info;
}

void enviarParaUSB(long info) {
  int tam  = sizeof(info);
  char buff[tam];
  memcpy(&buff, &info, tam);  

  Serial.write((uint8_t*)&buff, tam);
  //Serial.println(info);
}

int extrairDistancia(long info) {
  //
  int distancia = (info & 255);
  return distancia;
} 


int extrairStatusVaga(long info) {
  int statusVaga = (info & 65535) >> DESLOCAMENTO_STATUSVAGA;
  return statusVaga;
} 


int extrairUID(long info) {
  int uid = (info & 16777215) >> DESLOCAMENTO_RFID;
  return uid;
} 

void verificarLeitorRFID() {
  if( !rfid.PICC_IsNewCardPresent() ) {
    return;
  }
  if( !rfid.PICC_ReadCardSerial() ) {
    return;
  }
}

int pegarUID() {
 verificarLeitorRFID();
 int uid = rfid.uid.uidByte[0];
 return uid;
}

int pegarDistancia(){
  int microsec = ultrasonico.timing();
  int distancia;
  distancia = ultrasonico.convert(microsec, Ultrasonic::CM);
  return distancia;
}
