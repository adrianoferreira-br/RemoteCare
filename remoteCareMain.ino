

#include <WiFi.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <Ticker.h>
#include "remoteCarePins.h"



int alarmToSend = 0;


void setup() {
  Serial.begin(115200);
  setupDisplayOLED();
  initPersistence();  
  
  setupWifi();
  setupNtp();
  setupFirebase();
  
  
  sendToFirebaseIp();
  
  setupPins();
  startSensors();
  startBlinkLedTask();
  startWebServer();



}





void loop() {
if((WiFi.status() != WL_CONNECTED))
{
  Serial.println("wifi desconectada");
  setupWifi();
}
  updateNtp();
  processWebServer();

  loopDisplayOLED(); 
  if(alarmToSend != 0)
    mainSendToFireBase(alarmToSend);
  yield();
}








 
