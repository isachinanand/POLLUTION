#include <SoftwareSerial.h>
#define RX 10
#define TX 11

int sensorValue;
String AP = "SachinPoco";
String PASS = "HARHARMODI";

String API = "PHEBMOWB8YY9FMFE";
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";

int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor1 = 1;
int valSensor2 = 1;

SoftwareSerial esp8266(RX,TX);

void setup() {
  Serial.begin(9600);
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  
}

void loop() {
  valSensor1 = getSensor1Data();
  valSensor2 = getSensor2Data();
  String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(valSensor1) +"&"+ field2 +"="+String(valSensor2);

  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData); 
  delay(1500); 
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");

}
int getSensor1Data(){
  return analogRead(0);
}
int getSensor2Data(){
  return analogRead(1);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);
    if(esp8266.find(readReplay))
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false;
}
    
  
