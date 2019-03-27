//#include <Servo.h>
#include <SoftwareSerial.h>

// for information on how to set up the UDP Transmission, I used
// the following documentation, starting on page 4, and used the 
// example where the remote IP and port are fixed
// https://www.espressif.com/sites/default/files/documentation/4b-esp8266_at_command_examples_en.pdf

SoftwareSerial ESPserial(9,10); // connected to RX2 and TX2 at pins 
                          // 9 and 10 on the Teensy 3.5 per
                          // https://www.pjrc.com/teensy/td_uart.html


// TODO: send servo commands between Arduino and ESPSerial
// TODO: send wind sensor readings from ESPSerial to Arduino  
    // TODO: figure out how to get the encoder running
// TODO: send all data from ESPSerial to Arduino
// TODO: be able to move everything within the rigid wing based upon messages from the Arduino

//Pins for devices
const int onBoardLED = 13;


String SSID = "sailbot";
String PASS = "Passphrase123";
//String DST_IP = "192.168.0.21";
String DST_IP = "130.215.174.114";
int DST_PORT = 52011;
//int DST_PORT = 8888; // port on which the UDP communication will be
                      // taking place
String THIS_DEVICE_IP = "192.168.0.25";


void setup() {
  //init

  Serial.begin(115200);
  ESPserial.begin(115200);

  //Serial.println("Communication Initialized");
  //Serial.println("I just got into setup!!");
  pinMode(onBoardLED, OUTPUT);
  digitalWrite(onBoardLED, HIGH);
  //setWifiMode();
  
  // Initialize Everything
  //initializeComs();
  //initializeWifi();
  
}

void loop() {
  //Serial.println("I just entered the loop!");

  digitalWrite(onBoardLED,LOW);
  /*
   initializeUDP();
   digitalWrite(onBoardLED,HIGH);
   checkStatus();
   sendMessage();
   closeUDP();
    Serial.println("Uh oh. The ESP Serial bus isn't available");
  */

  if ( ESPserial.available() ){ 
      Serial.write(ESPserial.read());  
    }
 
    // listen for user input and send it to the ESP8266
    if ( Serial.available() ){  
      ESPserial.write(Serial.read());  
    }
}

// This initializes the serial buses
/*
void initializeComs() {
  Serial.begin(115200);
  ESPSerial.begin(115200);

  Serial.println("Communication Initialized");
}
*/

void setWifiMode(){
  // set the wifi mode to be in softAP and station mode
  ESPserial.println("AT+CWMODE=3");
  Serial.println("I just set up the wifi mode for the esp");
}

void initializeWifi(){
  // utilizes the built in serial commands between the esp and teensy
  // to start up the wifi connection with the sailbot server
  if(ESPserial.available()){
     ESPserial.println("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"");
     Serial.println("Wifi is officially initialized");
  }else{
    Serial.println("It didn't work :(");
  }
}

void initializeUDP(){
  // once again utilizes the AT commands that can be found at https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf
  // to send information to the esp to start UDP comms between
  // the esp and the arduino
  // this specific command is found on page 42 in the documentation

  // for this process, I've selected to only have one UDP channel
  // (we'll see if this changes)
  ESPserial.println("AT+CIPMUX=0"); // makes sure that it's only one connection
  //if(ESPSerial.read()== "OK"){
      ESPserial.println("AT+CIPSTART=\"UDP\",\"" + DST_IP + "\"," + DST_PORT);
    //  if(ESPSerial.read() == "OK"){
        Serial.println("just attempted to initialize UDP");
    //  }
 // } else {
    Serial.println("something went wrong with the UDP");
}

void sendMessage(){
  ESPserial.println("AT+CIPSEND=8");
  
}

void closeUDP(){
  ESPserial.println("AT+CIPCLOSE");
}

void checkStatus(){
  ESPserial.println("AT+CIPSTATUS");
}
 
