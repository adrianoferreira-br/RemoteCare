#include <WiFiUdp.h>
#include <NTPClient.h>


const char* ntpServerName = "a.st1.ntp.br"; //a.ntp.br";



byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets


#define FUSO              -3      // (utc+) TZ in hours
#define SUMMER_TIME          0//60      // use 60mn for summer time in some countries
#define FUSO_SECS           ((FUSO)*3600)

#define NTP_UPDATE_TIME_MS 800000


//WiFiUDP ntpUDP; // Declaração do Protocolo UDP
//NTPClient timeClient(ntpUDP, ntpServerName, FUSO_SECS, 60000);

void setupNtp() {
  //timeClient.begin();

  configTime(FUSO_SECS, SUMMER_TIME, ntpServerName);
}

void updateNtp()
{
 // timeClient.update();
}
