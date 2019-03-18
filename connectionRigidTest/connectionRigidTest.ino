//#include <Servo.h>

#define ESPSerial Serial2 // connected to RX2 and TX2 at pins 
                          // 9 and 10 on the Teensy 3.5 per
                          // https://www.pjrc.com/teensy/td_uart.html


// Things to work on:
// Be able to retrieve and read messages from Serial sent to Serial2
// and then be able to send those messages from Serial2 via TCP to 
// the arduino

// TODO: send message between ESPSerial and Arduino
// TODO: send servo commands between Arduino and ESPSerial
// TODO: send wind sensor readings from ESPSerial to Arduino  
    // TODO: figure out how to get the encoder running
// TODO: send all data from ESPSerial to Arduino
// TODO: be able to move everything within the rigid wing based upon messages from the Arduino


//Pins for devices
#define onBoardLED 13


#define SSID "sailbot"
#define PASS "Passphrase123"
#define DST_IP "192.168.0.21"
#define DST_PORT 80
#define THIS_DEVICE_IP "192.168.0.25"

int printing = 1;
int connectionCount = 0;
int failCount;

void setup() {
  //init
  failCount = 0;
  Serial.println("I just got into setup!!");
  pinMode(onBoardLED, OUTPUT);
  digitalWrite(onBoardLED, HIGH);
  
  // Initialize Everything
  initializeComs();
  initializeWifi();

  // Connect to the network
  connectToNetwork(SSID, PASS);
}

void loop() {
 
  digitalWrite(onBoardLED,LOW);
  
  if (Serial.available()) {
    Serial.println("I will read messages from your device in the future!");
  }
  if(ESPSerial.available()){
    Serial.println("I received messages from the ESP!");
  }
  
  if (connectedTCP()) {
    connectionCount = 0;
    digitalWrite(onBoardLED, HIGH);
  } else {
    connectionCount++;
    if (connectionCount >= 4) {
      openTCP(DST_IP, DST_PORT);    //if no message is received than there is no connection so the port is openend
      delay(50);
    }
  }
}


// This initializes the serial buses
void initializeComs() {
  Serial.begin(115200);
  ESPSerial.begin(115200);

  Serial.println("Communication Initialized");
}



// This initializes the ESP8266 module
int initializeWifi() {
  Serial.println("Trying to initialize wifi");
  // Reset the module
  sendMessageToESP("AT+RST");
  
  if (printing) Serial.println("Resetting Wifi Module");

  // wait for a "ready" command
  bool reset_successful = waitForStringESPSerial("ready", 3000);

  if (reset_successful) {
    if (printing) Serial.println("Wifi Reset Successfully");
    return 0;

  } else {
    if (printing) Serial.println("Wifi Reset Failed");
    return 1;
  }
}



// This scans for networks and returns a list of networks
int scanForNetworks() {
  // Send the command to print all nearby networks
  sendMessageToESP("AT+CWLAP");

  // TODO: print out all networks
  return 0;
}



// This searches for networks and returns true if the selected network is found
int searchForNetwork(String networkName) {
  return 0;
}



// This attempts to connect to a network. If it is succesful, True is returned
bool connectToNetwork(String ssid, String password) {

  if (printing) {
    Serial.println("Attempting to connect to " + ssid);
    Serial.println("Password is " + password);
  }

  // Maybe search for network to see it it's available first?

  // Set the operating mode to Client
  // Client = 1, AP = 2, Client and AP = 3
  sendMessageToESP("AT+CWMODE=1");

  // Build the message to connect to the given ssid with the password
  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  sendMessageToESP(cmd);

  // wait for a "OK" command
  bool connection_successful = waitForStringESPSerial("OK", 3000);

  cmd = "AT+CIPSTA=\"" THIS_DEVICE_IP "\"";
  sendMessageToESP(cmd);

  // wait for a "OK" command
  connection_successful = connection_successful && waitForStringESPSerial("OK", 3000);

  if (connection_successful) {
    if (printing) Serial.println("Connection Successful");
    return true;

  } else {
    if (printing) Serial.println("Connection Failed");
    return false;
  }
}



// Get ip address if it's connected to a network
String getIP() {
  sendMessageToESP("AT_CIFSR");

  // Sort out IP address
  return "0.0.0.0";
}



// Open a TCP connection
// A returned value of True indicates it was successful
boolean openTCP(String ip, int port) {
  // Set transparent mode to 1 so that messages recieved will be sent directly to serial
  // Set transparent mode to 0
  //  sendMessageToESP("AT+CIPMODE=0", printing);

  // build command
  String cmd = "AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port;

  sendMessageToESP(cmd);
  //  Serial.println(cmd);

  // wait for a "OK" command
  bool connection_successful = waitForStringESPSerial("OK", 3000);

  if (connection_successful) {
    if (printing) Serial.println("TCP Connection to " + ip + " port number " + String(port) + " successful");
    return true;
  } else {
    if (printing) Serial.println("TCP Connection to " + ip + " port number " + String(port) + " failed");
    return false;
  }
}



// Send a message over TCP()
void sendTCPMessage(String msg) {

  // build initial message
  String instructionToSend = "AT+CIPSEND=" + String(msg.length());

  if (printing) Serial.println("Sending message: " + msg);

  // Send the message
  sendMessageToESP(instructionToSend);
  delay(20);
  sendMessageToESP(msg);
}



// Close the current TCP connection
int closeTCP() {
  sendMessageToESP("AT+CIPCLOSE");

  if (printing) Serial.println("TCP Closed");

  return 0;
}



// Return true if connected to TCP, false otherwise
bool connectedTCP() {
  sendMessageToESP("AT+CIPSTATUS");

  if (waitForStringESPSerial("STATUS:3", 500)) {
    if (printing) Serial.println("TCP still connected");
   // tcpConnection = 1;
    return true;
  } else {
    failCount++;
    Serial.println(failCount);
    if (printing) Serial.println("TCP connection lost");
   // tcpConnection = 0;
   if(failCount == 10){
    sendMessageToESP("AT+RST");
    failCount = 0;
   }
    return false;
  }
}

// This method sends a message to the ESP from the Teensy
void sendMessageToESP(String commandToSend) {
  ESPSerial.println(commandToSend);

  if (printing >= 2) Serial.println("--- " + commandToSend);
}

// This method scans the input from ESPSerial for a specific key
// If this key is found before the timeout, true is returned.
// Othertime false is returned
bool waitForStringESPSerial(String key, int timeout) {

  int start_time = millis();

  while (millis() < start_time + timeout) {
    if (ESPSerial.available()) {
      String data = ESPSerial.readString();
      // Serial.println(data);

      for (int i = 0; i < data.length() - key.length(); i++) {
        if (data.substring(i, i + key.length()) == key) {
          return true;
        }
      }
    }
  }

  return false;
}  
