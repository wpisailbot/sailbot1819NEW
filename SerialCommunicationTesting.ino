// Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the ESP8266
//
 
#include <SoftwareSerial.h>
SoftwareSerial ESPserial(9, 10); // RX | TX

char msg = 'MODE';
int count = 1;
String DST_IP = "192.168.0.21";
int DST_PORT = 80; // port on which the UDP communication will be
                      // taking place
 
void setup() 
{
    Serial.begin(115200);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(115200);  
 
    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");    
}
 
void loop() 
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
//    if ( ESPserial.available() )   {  Serial.write( ESPserial.read() );  }
// 
//    // listen for user input and send it to the ESP8266
   if ( Serial.available() )       {  
//         ESPserial.write( "AT+CWMODE=3");
//         ESPserial.write('\r');
//         ESPserial.write( '\n');
//         ESPserial.flush();
//         delay(10);
        
//         // ESPserial.write("AT+CWJAP=?");
//          ESPserial.write("AT+CWJAP=\"sailbot\",\"Passphrase123\"");
//          ESPserial.write('\r');
//          ESPserial.write( '\n');
//          ESPserial.flush();
//          delay(10);
//
//          ESPserial.write("AT+CIPMUX=1");
//  ESPserial.write('\r');
//  ESPserial.write( '\n');
//  ESPserial.flush();
//  delay(10);
//
//          ESPserial.write("AT+CIPSTART=1,\"UDP\",\"192.168.0.21\",80,1112,0");
//   ESPserial.write('\r');
//   ESPserial.write( '\n');
//   ESPserial.flush();
//   delay(10);
//
//          ESPserial.write("AT+CIPSEND=1,2");
//   ESPserial.write('\r');
//   ESPserial.write( '\n');
//   delay(100);
//   ESPserial.flush();
//   delay(10);
//
//   ESPserial.write("ok");
//   ESPserial.write('\r');
//   ESPserial.write( '\n');
//   delay(100);
//   ESPserial.flush();
//   delay(10);

//          setNewMode();
          setWIFI();
          openChannel();
          startComms();
          sendInitMsg();
   
      
    
      }//Serial.read() );  }
}

void setNewMode(){
  ESPserial.write( "AT+CWMODE=3");
  ESPserial.write('\r');
  ESPserial.write( '\n');
  ESPserial.flush();
  delay(10);
}

void setWIFI(){
  ESPserial.write("AT+CWJAP=\"sailbot\",\"Passphrase123\"");
  ESPserial.write('\r');
  ESPserial.write( '\n');
  ESPserial.flush();
  delay(10);
}


void openChannel(){
  ESPserial.write("AT+CIPMUX=1");
  ESPserial.write('\r');
  ESPserial.write( '\n');
  ESPserial.flush();
  delay(10);
}

void startComms(){
   ESPserial.write("AT+CIPSTART=1,\"UDP\",\"192.168.0.21\",80,1112,0");
   ESPserial.write('\r');
   ESPserial.write( '\n');
   ESPserial.flush();
   delay(10);
}

void sendInitMsg(){
   ESPserial.write("AT+CIPSEND=1,2");
   ESPserial.write('\r');
   ESPserial.write( '\n');
   delay(100);
   ESPserial.flush();
   delay(10);

   ESPserial.write("no       ");
   ESPserial.write('\r');
   ESPserial.write( '\n');
   delay(100);
   ESPserial.flush();
   delay(10);
}
