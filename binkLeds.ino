Ticker tickerWifi;


bool blinkLeds[NUMBER_OF_BOXES]  = {false,false,false,false};;
int leds[NUMBER_OF_BOXES] = {LED_1_PIN, LED_2_PIN, LED_3_PIN, LED_4_PIN};


void setBlinkLed(int position, bool value)
{
  blinkLeds[position] = value;  
}


void blinker()
{

  for(int counter = 0; counter < NUMBER_OF_BOXES; counter++)
  {
    if(blinkLeds[counter])
    {
      digitalWrite(leds[counter], !digitalRead(leds[counter]));
    }
    else
    {
      digitalWrite(leds[counter],0);
    }
  }
}

void startBlinkLedTask()
{
  tickerWifi.attach(1.0 , blinker);
}


void blinkProg(int box)
{
  for(int i = 0; i<3;i++)
  {
    digitalWrite(leds[box - 1],1);
    delay(150);
    digitalWrite(leds[box - 1],0);
    delay(150);
  }
}
