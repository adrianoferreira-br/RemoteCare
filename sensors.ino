Ticker tickerSensors;
int counterPanic = 0;

void setupPins()
{
// pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);//buzzer

  pinMode(PANIC_BUTTON_PIN,INPUT_PULLUP);
 
  pinMode(SENSOR_1_PIN,INPUT_PULLUP);
  pinMode(SENSOR_2_PIN,INPUT_PULLUP);
  pinMode(SENSOR_3_PIN,INPUT_PULLUP);
  pinMode(SENSOR_4_PIN,INPUT_PULLUP);
  
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);


  //init

  //digitalWrite(LED_BUILTIN, 0);

  digitalWrite(BUZZER_PIN, 0);
}


void startSensors()
{
  tickerSensors.attach(0.3,sensorsRead);
}

String oldDadosSensors = "";

int oldPanicValue = 1;
void sensorsRead()
{
  int sensor1 = digitalRead(SENSOR_1_PIN);
  int sensor2 = digitalRead(SENSOR_2_PIN);
  int sensor3 = digitalRead(SENSOR_3_PIN);
  int sensor4 = digitalRead(SENSOR_4_PIN);
  int panic = digitalRead(PANIC_BUTTON_PIN);

  if(sensor1 == 1)
    boxOpened(1);
  if(sensor2 == 1)
    boxOpened(2);
  if(sensor3 == 1)
    boxOpened(3);
  if(sensor4 == 1)
    boxOpened(4);

  if(panic == 0)
  {
    alarmPanic(true);
    
    if(counterPanic++ == 10)
      alarmToSend = 4;
    oldPanicValue = 0;
  }
  else
  {
    if(oldPanicValue == 0)
      alarmPanic(false);
   counterPanic = 0;
   oldPanicValue = 1;
  }


  processSchedule();

  String dados ="";

  dados = "sensores: ";
  dados += sensor1;
  dados += ", ";
  dados += sensor2;
  dados += ", ";
  dados += sensor3;
  dados += ", ";
  dados += sensor4;
  dados += ", ";
  
  dados += ", Panic: ";
  dados += panic;

  if(oldDadosSensors != dados)
  {
    oldDadosSensors = dados;
    Serial.println(dados.c_str());
  }

  /*
  if(sensor1 == 1)
    digitalWrite(LED_1_PIN, 1);
  else
    digitalWrite(LED_1_PIN, 0);

  
  if(sensor2)
    digitalWrite(LED_2_PIN, 1);
  else
    digitalWrite(LED_2_PIN, 0);

  
  if(sensor3)
    digitalWrite(LED_3_PIN, 1);
  else
    digitalWrite(LED_3_PIN, 0);

  
  if(sensor4)
    digitalWrite(LED_4_PIN, 1);
  else
    digitalWrite(LED_4_PIN, 0);
  */
}

void sendPanicMessage()
{
  sendToFirebase("/Panic",1);
}
