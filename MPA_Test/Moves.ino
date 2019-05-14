void left90() {
  double startRichtung;
  startRichtung = richtung;
  float engineLevel;
  
 // while (winkelDifferenz(richtung,startRichtung) < 90) {
  while(true) {
      engineLevel = (float) (90 - winkelDifferenz(richtung,startRichtung)/2 + 55) / 100;
      setLeftDrive(-1);
      setRightDrive(1);
      if (winkelDifferenz(richtung,startRichtung) >= 90) {
      setLeftDrive(1);
      setRightDrive(-1);
      delay(100);
      stoppEngines();
      break;
    }
  }
  setLeftDrive(engine1Power);
  setRightDrive(engine2Power);
}


void stoppEngines() {
  setLeftDrive(-2 * engine1Power);
  setRightDrive(-2 * engine2Power);

    delay(100);
  haltEngines();
}

void haltEngines() {
  setLeftDrive(0);
  setRightDrive(0);
}

void printSensorValues() {
  Serial.print("Front Left: ");
  Serial.print(proxyFrontLeft);
  Serial.print(" Front right: ");
  Serial.print(proxyFrontRight);
  Serial.print(" Back Left: ");
  Serial.print(proxyRearLeft);
  Serial.print(" Back right: ");
  Serial.print(proxyRearRight);
  Serial.print(" leftRot: ");
  Serial.print(leftRotation);
  Serial.print(" rightRot: ");
  Serial.print(rightRotation);
  Serial.print(" direction: ");
  Serial.print(richtung);
  Serial.println();  
}

