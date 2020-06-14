
Ticker tickerAlarm;




void alarm() {
  // tone(BUZZER_PIN, f, 100);
   if(digitalRead(BUZZER_PIN))
    digitalWrite(BUZZER_PIN,0);//!digitalRead(BUZZER_PIN));
  else
    digitalWrite(BUZZER_PIN,1);//!digitalRead(BUZZER_PIN));
}

bool alarmEnabled = false;
void enableAlarm(bool value)
{
  
  if(value)
  {
    if(!alarmEnabled)
    {
      Serial.println("alarm on");
      alarmEnabled = true;
      tickerAlarm.attach(0.8, alarm);
    }
  }
  else
   {
    if(alarmEnabled)
    {
      Serial.println("alarm off");
      tickerAlarm.detach();
      digitalWrite(BUZZER_PIN, 0);
      alarmEnabled = false;
    }
   }
}


void alarmPanic(bool value)
{
  if(value)
    digitalWrite(BUZZER_PIN,1);
  else
  {
    alarmEnabled = true;
    enableAlarm(false);
  }

}
