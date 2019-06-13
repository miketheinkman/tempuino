/*
  Tempuino:
  
  Overtemp shutdown protection with reset capability for 50mil BTU oil-fueled forced-air water heater.
  
  When max temp is reached, fuel-oil is cut and cannot be restarted until temp is back in range 
  AND reset button is held for 2 seconds. Either condition alone will not restart oil.
  
  All collected data can later be used for remote monitoring/control/reporting purposes.
  
*/

// declarations
int sensorValue1 = 0;
int sensorValue2 = 0;
int shutdownCondition = 0;
int resetCount = 0;
int reportCount = 0;
int maxTemp = 215;
char status = 'n';

void setup()
{
  // temp sensors
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  // reset button
  pinMode(2, INPUT);
  
  // shutdownCondition Relay
  pinMode(13, OUTPUT);
  
  // allClear Relay
  pinMode(12, OUTPUT);
  
  // acc shutdown
  pinMode(8, INPUT_PULLUP);
  
  // initialize serial
  Serial.begin(9600);
}

void loop()
{
  // track loops to delay reporting
  reportCount++;
  
  // unset shutdownCondition
  if (digitalRead(2) == HIGH){resetCount++;}
  else {resetCount = 0;}
  if (resetCount >= 20 && sensorValue1 < maxTemp && sensorValue2 < maxTemp){
  shutdownCondition = 0;
  }
  
  // switch from shutdown condition to online
  if (shutdownCondition == 0){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  status = 'n';
  }
  
  // read temp sensors
  sensorValue1 = analogRead(A0);
  sensorValue2 = analogRead(A1);
  
  // if sensor values >= 215 or pin8 (key shutdown) reads high
  //set shutdown condition
  if (sensorValue1 >= maxTemp || sensorValue2 >= maxTemp){
  shutdownCondition = 1;
  status = 't';
  }
  
  if (digitalRead(8) == HIGH){
  shutdownCondition = 1;
  status = 'k';
  }
  
  // if shutdown condition send shutdown
  if (shutdownCondition == 1){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  }
  
  // print temp values, status every 20 iterations:
  if (reportCount >= 20){
  Serial.print(sensorValue1);
  Serial.print(",");
  Serial.print(sensorValue2);
  Serial.print(",");
  Serial.println(status);
  reportCount = 0;
  }
  
  // Slight delay for sanity
  delay(100); 
}
