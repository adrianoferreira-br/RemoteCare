// Create an instance of the server
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

bool alarmByWeb = false;

WebServer server(80);


void startWebServer()
{
   if (MDNS.begin("RemoteCare")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/settime.htm",setTimeOnBox);
  server.on("/gettime.htm",getTimeOnBox);
  server.on("/getReport.htm",getReport);
  server.on("/alarm.htm",setAlarm);
   server.on("/test.svg", drawGraph);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println(F("Server started"));
}


void processWebServer()
{
 // Serial.println("server.handleClient");
  server.handleClient();
}


void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}



void setAlarm()
{
  if(server.args() == 1)
  {
    if(server.arg(0) == "1")
    {
      enableAlarm(true);
      alarmByWeb = true;
    }
    else
    {
      enableAlarm(false);
      alarmByWeb = false;
    }
  }
  server.send(200, "text/plain", "setAlarm OK");
}

//arguments ?box1time=hh:mm&box2time=hh:mm&box3time=hh:mm&box4time=hh:mm&box1date=DD/MM&box2date=DD/MM&box3date=DD/MM&box4date=DD/MM
void setTimeOnBox()
{

  server.send(200, "text/plain", "setTimeOnBox OK");
  
  for (uint8_t i = 0; i < server.args(); i++) {
		if(server.argName(i) == "box1time")
		{
		  boxTime[0] = server.arg(i);
      blinkProg(1);
		}
		else if(server.argName(i) == "box2time")
		{
		  boxTime[1] = server.arg(i);
     blinkProg(2);
		}
		else if(server.argName(i) == "box3time")
		{
		  boxTime[2] = server.arg(i);
     blinkProg(3);
		}
		else if(server.argName(i) == "box4time")
    {
			boxTime[3] = server.arg(i);
     blinkProg(4);
    }
		else if(server.argName(i) == "box1date")
			boxDate[0] = server.arg(i);
		else if(server.argName(i) == "box2date")
			boxDate[1] = server.arg(i);
		else if(server.argName(i) == "box3date")
			boxDate[2] = server.arg(i);
		else if(server.argName(i) == "box4date")
			boxDate[3] = server.arg(i);
  }
	persistScheduleBoxes();	 
	
}

void getReport()
{
	
  String retorno;
  retorno = getPesistedReport();


   server.send(200, "text/plain", retorno.c_str());
}

//returns  json array
void getTimeOnBox()
{
	String result = "";

	result="{\"remotecare\"[";
	
	result += "{\"box\":\"1\",";
	result += "\"hora\":\"";
	result += boxTime[0];
	result += "\",";
	result += "\"data\":\"";
	result += boxDate[0];
	result += "\"}";
		
	result +=",";

	result += "{\"box\":\"2\",";
	result += "\"hora\":\"";
	result += boxTime[1];
	result += "\",";
	result += "\"data\":\"";
	result += boxDate[1];
	result += "\"}";

	result +=",";

	result += "{\"box\":\"3\",";
	result += "\"hora\":\"";
	result += boxTime[2];
	result += "\",";
	result += "\"data\":\"";
	result += boxDate[2];
	result += "\"}";

		result +=",";

	result += "{\"box\":\"4\",";
	result += "\"hora\":\"";
	result += boxTime[3];
	result += "\",";
	result += "\"data\":\"";
	result += boxDate[3];
	result += "\"}";
	
	result+="]}";

  server.send(200, "text/plain", result.c_str()); 
}

/*
 void processWebServer1()
 {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(10000); // default is 1000
  Serial.println(F("new client pos timeout"));
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);



  ProcessHttpRequest(req);

  int sensor1 = digitalRead(SENSOR_1_PIN);
  int sensor2 = digitalRead(SENSOR_2_PIN);
  int sensor3 = digitalRead(SENSOR_3_PIN);
  int sensor4 = digitalRead(SENSOR_4_PIN);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  char buffer[80];
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80," %d %B %Y %H:%M:%S ",timeinfo);

  
  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r "));
  
  client.print(F("<br>sensor 1: "));
  client.print((sensor1) ? F("ativado") : F("desativado"));
  client.print(F("<br>sensor 2: "));
  client.print((sensor2) ? F("ativado") : F("desativado"));
  client.print(F("<br>sensor 3: "));
  client.print((sensor3) ? F("ativado") : F("desativado"));
  client.print(F("<br>sensor 4: "));
  client.print((sensor4) ? F("ativado") : F("desativado"));
  
  client.print(F("<br>Data: "));
  client.print(buffer);
  client.print(F("<br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_1/on'>acende o led 1</a> "));

  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_1/off'>apaga o led 1</a> "));

  client.print(F("<br>Data: "));
  client.print(buffer);
  client.print(F("<br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_2/on'>acende o led 2</a> "));

  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_2/off'>apaga o led 2</a> "));

  client.print(F("<br>Data: "));
  client.print(buffer);
  client.print(F("<br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_3/on'>acende o led 3</a> "));

  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_3/off'>apaga o led 3</a> "));

    client.print(F("<br>Data: "));
  client.print(buffer);
  client.print(F("<br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_4/on'>acende o led 4</a> "));

  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/led_4/off'>apaga o led 4</a> "));
  





  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/imperialmarch'>Musica</a>.<br>"));
  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/alarmon'>Liga alarme</a>.<br>"));
  
  client.print(F("<br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/alarmoff'>Desliga alarme</a>.<br>"));
   
  client.print(F("</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));

}
*/





void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);

}

void handleRoot() {

  char temp[400];
  char buffer[80];
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80," %d %B %Y %H:%M:%S ",timeinfo);

  snprintf(temp, 400,

  "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>RemoteCare</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from RemoteCare!</h1>\
    <p>Date: %s</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>"
,buffer);

  server.send(200, "text/html", temp);
}
