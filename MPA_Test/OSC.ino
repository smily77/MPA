void push1(OSCMessage &msg, int addrOffset) {
  digitalWrite(beeper, (boolean) msg.getFloat(0));
}

void toggleOnOff(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getFloat(0);
  digitalWrite(ledPin, ledState);               // External LED 
  ledState = !ledState;     // toggle the state from HIGH to LOW to HIGH to LOW ...
}

void fader1(OSCMessage &msg, int addrOffset){
  engine1Power = msg.getFloat(0);
  setLeftDrive(engine1Power);
}

void fader2(OSCMessage &msg, int addrOffset){
  engine2Power = msg.getFloat(0);
  setRightDrive(engine2Power);
}

void sendMsG() {
  OSCMessage msgOut("/1/direction");
  msgOut.add(engine1Power);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

void HB() {
  HBflag = !HBflag;
  OSCMessage msgOut("/1/led1");
  msgOut.add(HBflag);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
  //delay(100);
}

void sendDirection() {
  OSCMessage msgOut("/1/direction");
  msgOut.add( (int) richtung);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
  l_richtung = richtung;
}

void sendFLP() {
  OSCMessage msgOut("/1/FLled");
  msgOut.add(FLP);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

void sendFRP() {
  OSCMessage msgOut("/1/FRled");
  msgOut.add(FRP);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

void sendRLP() {
  OSCMessage msgOut("/1/RLled");
  msgOut.add(RLP);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

void sendRRP() {
  OSCMessage msgOut("/1/RRled");
  msgOut.add(RRP);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

void sendLeftDist(){
  OSCMessage msgOut("/1/distLeft");
  msgOut.add(leftRotation);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

void sendRightDist(){
  OSCMessage msgOut("/1/distRight");
  msgOut.add(rightRotation);
  Udp.beginPacket(destIp, destPort);
  msgOut.send(Udp);
  Udp.endPacket();
  msgOut.empty(); 
}

