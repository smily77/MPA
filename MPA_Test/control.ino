void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 100) {
  uint32_t duty = (LEDC_BASE_FREQ / valueMax) * value;
  if (value > valueMax) duty = LEDC_BASE_FREQ;
  ledcWrite(channel, duty);
}

void systemTimerFireExecute() {
  portENTER_CRITICAL(&timerMux);
  isrCount = isrCounter;
//  isrTime = lastIsrAt;
  portEXIT_CRITICAL(&timerMux);

  digitalWrite(bottomProxyPower,HIGH);
  bno.getEvent(&event);
  richtung = event.orientation.x;
  proxyFrontLeft = analogRead(bottomProxyFrontLeft);
  FLP = (proxyFrontLeft > blackLevel);  
  proxyFrontRight = analogRead(bottomProxyFrontRight);
  FRP = (proxyFrontRight > blackLevel);    
  proxyRearLeft = analogRead(bottomProxyBackLeft);
  RLP = (proxyRearLeft > blackLevel);    
  proxyRearRight = analogRead(bottomProxyBackRight);
  RRP = (proxyRearRight > blackLevel);  
  digitalWrite(bottomProxyPower,LOW);
  
  if ((isrCount / 100) * 100 == isrCount) timerFire100flag = true;
}

void didShot() {
  digitalWrite(23,LOW);
  detachInterrupt(33);
}



void setLeftDrive(float x) {
    engine1Power = x;
    if (x < (-1* noResponseZone)) {
    ledcAnalogWrite(LEDC_CH_1l,-100*x);
    ledcAnalogWrite(LEDC_CH_1h,0);
    leftDriveForward = false;
  }
  else if (x > noResponseZone){
    ledcAnalogWrite(LEDC_CH_1l,0);
    ledcAnalogWrite(LEDC_CH_1h,100*x);
    leftDriveForward = true;
  }
  else {
    ledcAnalogWrite(LEDC_CH_1l,0);
    ledcAnalogWrite(LEDC_CH_1h,0);
  }
}

void setRightDrive(float x) {
    engine2Power = x;
    if (x < (-1* noResponseZone)) {
    ledcAnalogWrite(LEDC_CH_2l,-100*x);
    ledcAnalogWrite(LEDC_CH_2h,0);
    rightDriveForward = false;
  }
  else if (x > noResponseZone){
    ledcAnalogWrite(LEDC_CH_2l,0);
    ledcAnalogWrite(LEDC_CH_2h,100*x);
    rightDriveForward = true;
  }
  else {
    ledcAnalogWrite(LEDC_CH_2l,0);
    ledcAnalogWrite(LEDC_CH_2h,0);
  }
}



