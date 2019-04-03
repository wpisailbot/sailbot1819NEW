#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
#include <EthernetUdp.h>
#include <Servo.h>
#include <SoftwareSerial.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

Servo rudderServo;

int state;

const int servo_pin = 9;

IPAddress ip(192, 168, 0, 21);

EthernetServer server(80);

EthernetUDP Udp;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

unsigned int localPort = 80;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Set up the server on the arduino
  Ethernet.begin(mac, ip);
  
   // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  
  server.begin();

  // set up the udp connection to send messages to the teensy
  Udp.begin(localPort);
  
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("UDP communication is located at port ");
  Serial.println(localPort);

  //inputString.reserve(200);
}

void loop() {
  // listen for incoming clients


 if(Serial1.available()){

  state = Serial1.read() - 48;

  Serial.println(state);
 }
  
  int packetSize = Udp.parsePacket(); // works here

  
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
  
    Serial.print(", port ");
    Serial.println(Udp.remotePort()); // works here
    
    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // works here
    
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    if(state == 1){
      Udp.write("1");
    } else if (state == 2){
      Udp.write("2");
    } else{
      Udp.write("3");
    }

    Udp.endPacket();
    Serial.println("sent same UDP packet");

    EthernetClient client = server.available();
  
    if (client) {    
     // create the http page  
     Serial.println("new client");
     
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("<h1>Output Values</h1>");
          for (int digitalChannel = 2 ; digitalChannel < 10; digitalChannel++) {
            int sensorReading = digitalRead(digitalChannel);
            client.print("Output value ");
            client.print(digitalChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    
    // close the connection:
    client.stop();

    }
    delay(10);
    
    Serial.print("I got: ");
    Serial.println(state);
  }
}

