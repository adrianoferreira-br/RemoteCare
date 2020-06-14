
#ifndef STASSID
#define STASSID "Senac Auditorio"
#define STAPSK  "@senac2018123"
#endif


const char wifiSSID[3][50] = {"Senac Auditorio","AdailtonSala","SSidAdr"};
const char wifiPassword[3][50] = {"@senac2018123","01121966","a12345678"};

void setupWifi()
{
  String frase;
  int wifiSSISIndex = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID[wifiSSISIndex], wifiPassword[wifiSSISIndex]);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  frase = "Conectando em ";
  frase += wifiSSID[wifiSSISIndex];
  Serial.print(frase.c_str());
  

  drawTextToOled(frase);

  int wifiCounter = 0;
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
    if(wifiCounter++ == 20) {
      wifiCounter = 0;
      if(wifiSSISIndex++ > 2)
        wifiSSISIndex = 0;
      WiFi.disconnect(false);
      WiFi.begin(wifiSSID[wifiSSISIndex], wifiPassword[wifiSSISIndex]);

      Serial.println();
      
      frase = "Conectando em ";
      frase += wifiSSID[wifiSSISIndex];
      Serial.print(frase.c_str());
  

      drawTextToOled(frase);

    }
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

    // Print the IP address
 
  frase = "Wifi conectado. IP: ";
  frase += WiFi.localIP().toString().c_str();
  Serial.println(frase.c_str());


  drawTextToOled(frase);

  
}
