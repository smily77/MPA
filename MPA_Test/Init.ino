void setAnalogOut() {
    ledcSetup(LEDC_CH_1l, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(ch1low, LEDC_CH_1l);
    ledcSetup(LEDC_CH_1h, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(ch1high, LEDC_CH_1h);
    ledcSetup(LEDC_CH_2l, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(ch2low, LEDC_CH_2l);
    ledcSetup(LEDC_CH_2h, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(ch2high, LEDC_CH_2h);
}

void setSystemTimer() {
  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 10000, true); //1S

  // Start an alarm
  timerAlarmEnable(timer);
}

void setWifi() {
  WiFi.config(IPAddress(192,168,0,232),IPAddress(192,168,0,2), IPAddress(255,255,255,0)); 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setUDP() {
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
}

void initBNO() {
     if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
    bno.setExtCrystalUse(true);
}

void setInternalPorts() {
  pinMode(ledPin, OUTPUT);
  pinMode(bottomProxyPower,OUTPUT);
  pinMode(bottomProxyFrontRight,INPUT);
  pinMode(bottomProxyFrontLeft,INPUT);
  pinMode(bottomProxyBackRight,INPUT);
  pinMode(bottomProxyBackLeft,INPUT);
  pinMode(frontLight, OUTPUT);
  pinMode(button0,INPUT_PULLUP);
  pinMode(beeper,OUTPUT);
  pinMode(hallRight,INPUT_PULLUP);
  pinMode(hallLeft, INPUT_PULLUP);
  pinMode(APDS9930_INT, INPUT);
}

void initAPDS() {
   // Initialize APDS-9930 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9930 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }
  
  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_2X) ) {
    Serial.println(F("Something went wrong trying to set PGAIN"));
  }

 // Set proximity interrupt thresholds
  if ( !apds.setProximityIntLowThreshold(PROX_INT_LOW) ) {
    Serial.println(F("Error writing low threshold"));
  }
  if ( !apds.setProximityIntHighThreshold(PROX_INT_HIGH) ) {
    Serial.println(F("Error writing high threshold"));
  }
  
  // Start running the APDS-9930 proximity sensor (interrupts)
  if ( apds.enableProximitySensor(true) ) {
    Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }

  
}

