#include <Servo.h>
#include <SoftwareSerial.h>

// Author: Sierra Palmer
// started 3/26/19

// for information on how to set up the UDP Transmission, I used
// the following documentation, starting on page 4, and used the 
// example where the remote IP and port are fixed
// https://www.espressif.com/sites/default/files/documentation/4b-esp8266_at_command_examples_en.pdf

// state machine for the rigid sail that will allow 
// for the rigid sail to perform different functions
// based upon the state number that it receives from
// the arduino

// there are going to be 6 states:
// 1. max lift
// 2. max drag
// 3. zero lift
// 4. fully feathered
// 5. changing heel angle
// 6. changing tack

// to solve for the required angles for the max lift 
// and drag, we used the charts at http://airfoiltools.com/airfoil/details?airfoil=joukowsk0018-jf

// from there, we had to calculate for the angle per 
// the potentiometer units of 0-1023 (analog)
// 1023/360 is 2.842 digits per degree
// therefore
// 7 degrees is equivalent to 19.894 or 1003.16
// 15 degrees is equivalent to 42.63 or 980.37

// give these both a buffer of two integers
// 7 degrees = 19-21 or 1002-1004
// 15 degrees = 42-44 or 979-981

Servo trimTab;


SoftwareSerial ESPserial(9,10); // connected to RX2 and TX2 at pins 
                          // 9 and 10 on the Teensy 3.5 per
                          // https://www.pjrc.com/teensy/td_uart.html


const int redLED = 4;
const int yellowLED = 5;
const int white1LED = 7;
const int white2LED = 8;

int count = 0;

const int onBoardLED = 13;

int val;

int maxLiftDegs = 7;
int maxDragDegs = 15;
int inIrons = 100;

int windSensePin = 38;

int trimTabPin = 0;//6;

int state;

int boatMsgState = 2;

int pos = 55;

int deadAhead = 730;

String line;

int incoming;

char printBuff[11];

String SSID = "sailbot";
String PASS = "Passphrase123";
String DST_IP = "192.168.0.21";
int DST_PORT = 80; // port on which the UDP communication will be
                      // taking place
String THIS_DEVICE_IP = "192.168.0.25";

void setup() {
  Serial.begin(57600); // need this for the messaging portion
                        // when it gets added in 
  ESPserial.begin(57600);

  
  Serial.println("Remember to to set Both NL & CR in the serial monitor.");
  Serial.println("Ready");  
  
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(white1LED, OUTPUT);
  pinMode(white2LED, OUTPUT); 
  pinMode(onBoardLED, OUTPUT);
  digitalWrite(onBoardLED, HIGH);
  
  //setWifiMode();
  ESPserial.println("AT+CWMODE=3\r\n");
  delay(1000);

  // Initialize Everything
  initializeWifi(); 

  delay(2000);

  //trimTab.attach(trimTabPin); //the pin for the servo control, and range if desired

  // need to work on timing here again so it doesn't just go
  // straight into the loop without connecting
  Serial.println("got to the end of the setup");
}

// read through the connection stuff from the old code

void loop() {
  
 ESPserial.listen();
// initializeWifi();
 while (ESPserial.available() > 0){
    //delay(2000);
    processEsp();
    if(count == 0){
      openUDPChannel();
      //delay(1000);
      processEsp();
      delay(1000);
      initializeUDP();
      processEsp();
      count = 1;
    } else if (count == 1){
      for(int i = 0; i < 20; i++){
        sendMessage();
        sendData();
      }
      count = 2;
    } else {
      
    }
  }

  processEsp();
  //findState();
  // listen for communication from the ESP8266 and then write it to the serial monitor
  // have receiving a message be an interrupt
  // receive a number message from the main boat
  state = boatMsgState;
  /*
  if(state == 0){
    while(boatMsgState == 0){
      // turn on a certain LED pattern to notify 
      // the shore that the boat comms are down
      Serial.println("no message was received");
      
      turnOnLEDs();
    }
  } else {
  runSail();
  }
  */
  //closeUDP(); 
  //processEsp();
}

  //Serial.println("WIFI WAS NOT CONNECTED");
  //}


void runSail(){
  switch(state)
  { case 1:
      Serial.println("you're asking for max lift");
     // turnOffLEDs();
      digitalWrite(redLED, HIGH);
      for(int i = 0; i < 20; i++){
        sendMessage();
        sendState1();
      }
      // lift is the component of the force perpendicular
      // to the oncoming flow direction
      // need to read out the 
      val = analogRead(windSensePin);
      if(val > deadAhead){
        trimTab.write(113);
      } else {
        trimTab.write(87);
      }
      
    break;
    
    case 2:
      //Serial.println("you're asking for max drag");
      turnOffLEDs();
      digitalWrite(yellowLED, HIGH);
      for(int i = 0; i < 20; i++){
        sendMessage();
        sendState2();
      }
      // drag is the component of the force parallel to 
      // the flow direction
      val = analogRead(windSensePin);
      if(val > deadAhead){
        trimTab.write(128);
      } else {
        trimTab.write(72);
      }
    break;
    
    case 3:
      Serial.println("you're asking for zero lift");
      turnOffLEDs();
      digitalWrite(redLED, HIGH);
      digitalWrite(yellowLED, HIGH);
      for(int i = 0; i < 20; i++){
        sendMessage();
        sendState3();
      }
      // "into irons" or sail directly into the wind
      // have the servo stay in line with the wind 
      // sensor
      trimTab.write(inIrons);
    break;
    
    case 4:
      Serial.println("you're asking for fully feathered");
      turnOffLEDs();
      digitalWrite(white2LED, HIGH);
      // sailing upwind so close to the wind that the forward 
      // edge of the sail is stalling or luffing, slightly thus
      // reducing the power generated by the sail and the 
      // angle of heel without stalling completely
    break;
    
    case 5:
      Serial.println("you're changing the heel angle");
      turnOffLEDs();
      digitalWrite(redLED, HIGH);
      digitalWrite(white1LED, HIGH);
      // want to move the boat away from the wind
      // moving the trim tab to go opposite to the wind
      // direction
    break;
    
    case 6:
      Serial.println("you're changing the tack");
      turnOffLEDs();
      digitalWrite(yellowLED, HIGH);
      digitalWrite(white2LED, HIGH);
      // not completely sure as to how the boat should be
      // doing at this point
    break;
  }
}

void turnOffLEDs(){
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(white1LED, LOW);
  digitalWrite(white2LED, LOW);
}
void turnOnLEDs(){
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(white1LED, HIGH);
  digitalWrite(white2LED, HIGH);
}

void setWifiMode(){
  // set the wifi mode to be in softAP and station mode
  for(int i = 0; i < 25; i++){
    ESPserial.println("AT+CWMODE=3\r\n");
    delay(100);
  }
}

void initializeWifi(){
  // utilizes the built in serial commands between the esp and teensy
  // to start up the wifi connection with the sailbot server
  for(int i = 0; i < 25; i++){
     ESPserial.println("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"\r\n");
     delay(100);
  }
}

void openUDPChannel(){
  // once again utilizes the AT commands that can be found at https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf
  // to send information to the esp to start UDP comms between
  // the esp and the arduino
  // this specific command is found on page 42 in the documentation
  // for this process, I've selected to only have one UDP channel
  
    ESPserial.print("AT+CIPMUX=1\r\n"); // makes sure that it's only one connection
   
}

void initializeUDP(){
 // for(int i = 0; i < 25; i++){
  ESPserial.println("AT+CIPSTART=1,\"UDP\",\"" + DST_IP + "\"," + DST_PORT + ",1112,0\r\n");
  delay(4);
 // }
}

void sendMessage(){
  
    ESPserial.print("AT+CIPSEND=1,6\r\n");
    
}

void sendData(){

    ESPserial.print("1\r\n");
   
}

void sendState1(){

    ESPserial.print("1\r\n");
   
}

void sendState2(){

    ESPserial.print("2\r\n");
   
}

void sendState3(){
    ESPserial.print("3\r\n");
}

// need to close the UDP portal
// need to be able to read the number that I'm sent

void closeUDP(){
 
  ESPserial.println("AT+CIPCLOSE=1\r\n");

}

void checkStatus(){
  
  ESPserial.print("AT+CIPSTATUS\r\n");
 
}

void processEsp(){
  while(ESPserial.available()){ // was > 0
    line = ESPserial.readStringUntil('\n');
   // processLine(line);
  }
}

void processLine(String line){
  Serial.println(line);
}


void findState(){
  processEsp();
  line.toCharArray(printBuff, 13);
  boatMsgState = printBuff[13];
  Serial.println(boatMsgState - 18);
}
