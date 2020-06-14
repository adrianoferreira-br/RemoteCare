//#include <IOXhop_FirebaseESP32.h>
//#include <IOXhop_FirebaseStream.h>
#include <FirebaseESP32.h>



/**************** FIREBASE  *****************************/
#define FIREBASE_HOST "remotecare-esp32.firebaseio.com"
#define FIREBASE_AUTH "BpUO3x4Wc4WmTeOYGgczXENUajdba1N2krUYoinC"


//#define FIREBASE_HOST "remotecarebr.firebaseio.com"
//#define FIREBASE_AUTH "dgQ2cJ3iR3DhXv1K4Ab4hRGDJoHKNpB7rccVISZ0"


int cnt_firebase = 0;


FirebaseData firebaseData1;

FirebaseData firebaseData2;

void setupFirebase(){
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    
  //  Firebase.stream("Numero", streamCallback);
}


void sendToFirebaseIp(){  

    Serial.print("sendToFirebaseIp: " );
    Serial.println(WiFi.localIP().toString().c_str());
    if(Firebase.setString(firebaseData1, "/Ip", WiFi.localIP().toString().c_str()))
      Serial.println("Firebase.setString ok");
    else
      Serial.println("Firebase.setString error");
     String ipOnfirebase = "";

     if(Firebase.getString(firebaseData2,"/Ip"))
       Serial.println( firebaseData2.stringData().c_str());
     else
      Serial.println("erro lendo firebase");
 
}




void sendToFirebase(String Nome, double value){  

    Serial.println("Set float value: "+ Nome + " = " + String(value));
    if(Firebase.setFloat(firebaseData1,Nome.c_str(), value ))
      Serial.println("sendToFirebase ok");
    else
      Serial.println("Erro em sendToFirebase");
  
 
}


void mainSendToFireBase(int tipo)
{
  if(tipo == 1)
    sendToFirebase("/Remote02",1.0);
  else if(tipo == 2)
    sendToFirebase("/Remote01",1.0);
  else if(tipo == 3)
     sendToFirebase("/Remote01",2.0);
  else if(tipo == 4)
     sendToFirebase("/Panic",1.0);
  else if(tipo == 5)
  {
    sendToFirebase("/Remote01",0.0);
    sendToFirebase("/Remote02",0.0);
  }

  alarmToSend = 0;   
}



/*
void streamCallback(streamResult event) {
  String eventType = event.eventType();
  eventType.toLowerCase();
  if (eventType == "put") {
    Serial.println("The stream event path: " + event.path() + ", value: " + String(event.getFloat()));
    Serial.println();
  }

}
*/



double randomDouble(double minf, double maxf)
{
  return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
}
