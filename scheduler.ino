
extern bool alarmByWeb;


int timeIsUp[NUMBER_OF_BOXES] = {0, 0, 0, 0};

bool timeToAlert[NUMBER_OF_BOXES] = {false, false, false, false};

bool timeToAlertRemote[NUMBER_OF_BOXES] = {false, false, false, false};

bool boxWasOpened[NUMBER_OF_BOXES] = {false, false, false, false};


void processSchedule()
{
  char currentTime[80];
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (currentTime,80,"%H:%M ",timeinfo);

  int currentMinutes = timeinfo->tm_min + timeinfo->tm_hour * 60;

  for(counter = 0; counter < NUMBER_OF_BOXES; counter++)
  {
    
    if(boxTime[counter].length() > 0)
    {
      int minutes = 0;
      int hour = 0;
      int boxtime = 0;
      //sscanf(boxTime[counter].c_str(), "%02d:%02d", &hour, &minutes);

      char temp[20];
      strcpy(temp,boxTime[counter].c_str());
      
      char * item = strtok(temp, ":");
      if(item != NULL)
      {
        char *item1 = item;
        if(*item1 == '0')
          item1++;
        hour = atoi(item1);
        item = strtok(NULL, ":");
        if(item != NULL)
        {
          item1 = item;
          if(*item1 == '0')
          item1++;
          minutes = atoi(item1);
        }
      }
       
      boxtime = minutes + hour * 60; 
      String message;
      message = "sched ";
      message += counter;
      message += " boxtime: ";
      message += boxtime;
      message += " currentMinutes: ";
      message += currentMinutes;
      //if(counter == 0)
      //  Serial.println(message.c_str());
      
      if((currentMinutes - boxtime) == 0 && timeIsUp[counter] == 0)
      {
        Serial.println("timeIsUp");
        timeIsUp[counter] = 1;
        setBlinkLed(counter,true);
        sendLocalAlert();
      }
      else if((boxtime - currentMinutes) == 5 && !timeToAlert[counter])
      {
        Serial.println("timeToAlert");
        timeToAlert[counter] = true;
       // sendPreAlert();
      }
      else if((currentMinutes - boxtime ) > 5 && timeIsUp[counter] && !timeToAlertRemote[counter])
      {
        Serial.println("timeToAlertRemote");
        timeToAlertRemote[counter] = true;

        sendAlertRemote();
      }

      if(boxWasOpened[counter])
      {
        Serial.println("boxWasOpened");
        
        if(timeIsUp[counter] == 1)
          sendResetAlert();
        if((currentMinutes - boxtime) > 1)
          timeIsUp[counter] = 0;
        else
          timeIsUp[counter] = 2;
       
          
        timeToAlert[counter] = false;
        boxWasOpened[counter] = false;
        setBlinkLed(counter,false);

        
      }
      
    }

  
  }
  for(counter =0; counter < NUMBER_OF_BOXES; counter++)
  {
    if(timeIsUp[counter] == 1)
    {
      enableAlarm(true); 
      return;
    }
  }
  if(digitalRead(PANIC_BUTTON_PIN) == 1 && !alarmByWeb)
    enableAlarm(false);
}


void boxOpened( int box)
{
  boxWasOpened[box-1] = true;
}


void sendResetAlert()
{
  alarmToSend = 5;
}


void sendAlertRemote()
{
  alarmToSend = 1;
}

void sendPreAlert()
{
  alarmToSend = 2;
}

void sendLocalAlert()
{
   alarmToSend = 3;
}
