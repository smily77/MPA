#include <WiFi.h>
#include <OSCBundle.h> 
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <APDS9930.h>

char ssid[] = "xxxxx";                 // your network SSID (name)
char pass[] = "xxxxxx";              // your network password

#define ch2low 27
#define ch2high 25
#define ch1low 12
#define ch1high 13
#define frontLight 14
#define bottomProxyPower 26
#define bottomProxyFrontRight 36
#define bottomProxyFrontLeft 34
#define bottomProxyBackRight 35
#define bottomProxyBackLeft 39
#define button0 0
#define beeper 2
#define hallRight 17
#define hallLeft 16
#define APDS9930_INT 15   
#define aux23 23
#define aux33 33

#define blackLevel 3200

#define LEDC_CH_1l 0
#define LEDC_CH_1h 1
#define LEDC_CH_2l 2
#define LEDC_CH_2h 3
#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

#define PROX_INT_HIGH   600 // Proximity level for interrupt
#define PROX_INT_LOW    0  // No far interrupt

#define noResponseZone 0.1

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int ledPin = 5;                 

WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
const unsigned int localPort = 8888;   // local port to listen for UDP packets at the NodeMCU (another device must send OSC messages to this port)
const IPAddress destIp(192,168,0,53);
const unsigned int destPort = 9000;    // remote port of the target device where the NodeMCU sends OSC to

APDS9930 apds = APDS9930();

sensors_event_t event;

//Timer
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

//Variabelen
unsigned int ledState = 1;             // LOW means led is *on*
float engine1Power,engine2Power;

int leftRotation;
boolean leftRotationFlag = false;
boolean leftDriveForward = false;
int rightRotation;
boolean rightRotationFlag = false;
boolean rightDriveForward = false;
boolean driveFlag = false;

double richtung; // global: aktuelle Richtung
double l_richtung = 999;


int proxyFrontLeft, proxyFrontRight, proxyRearLeft, proxyRearRight; // global: bondenAbstand
boolean FLP = false; 
boolean FRP = false;
boolean RLP = false; 
boolean RRP = false;
boolean l_FLP = false;
boolean l_FRP = false;
boolean l_RLP = false;
boolean l_RRP = false;
boolean HBflag = false;
boolean timerFire100flag = false;


unsigned long isrCount; // global: No. of systemTimer triggers

void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
  systemTimerFireExecute();
}


void setup() {
  Serial.begin(115200);
  setAnalogOut();    
  setWifi();
  setUDP();
  initBNO();
  initAPDS();
  setInternalPorts();
  setSystemTimer();
  digitalWrite(frontLight, LOW);
  digitalWrite(bottomProxyPower,LOW);
  digitalWrite(beeper,LOW);
  leftRotation = 0;
  rightRotation = 0;  
  attachInterrupt(hallRight,hR,FALLING);
  attachInterrupt(hallLeft,hL,FALLING);

  pinMode(aux23, OUTPUT);
  digitalWrite(aux23, LOW);
  pinMode(aux33, INPUT_PULLUP);
}

void hR() {
  rightRotation++;
  rightRotationFlag = true;
  
}

void hL() {
  leftRotation++;
  leftRotationFlag = true;
}


void loop() {
 
  OSCMessage msgIN;
  int size;
  if((size = Udp.parsePacket())>0){
    while(size--)
      msgIN.fill(Udp.read());
      if(!msgIN.hasError()){
        msgIN.route("/1/toggleLED",toggleOnOff);
        msgIN.route("/1/fader1",fader1);
        msgIN.route("/1/fader2",fader2);
        msgIN.route("/1/push1",push1);
      }
  }
  if (timerFire100flag){
    HB();
//  sendDirection();
//  sendRightDist();
    delay(100);
//  sendLeftDist();
    timerFire100flag = false;
    printSensorValues();
  }

}



