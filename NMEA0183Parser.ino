// Author: Sierra Palmer
// Started 3/26/19

// skeleton code for parsing through the NMEA 0183 messages
// that are converted using the Maretron USB100

// all of these are created utiliizng the documentation found
// at http://freenmea.net/docs for the NMEA0183 sentences

// stores the current time of when the messages are being
// received in UTC
String hours[2];
String minutes[2];
String seconds[5];

// checks the latitude and whether the boat is facing north
// if northCheck is false, the boat is facing south
String latitude[6];
boolean northCheck;

// checks the longitude and whether the boat is facing east
// if eastCheck is false, the boat is facing west
String longitude[6];
boolean eastCheck;

// GPS Quality Indicator
// 0 - fix not available
// 1 - GPS fix
// 2 - Differential GPS fix
String GPSQual;

// checks to see how many satellites are in view
// from 00 to 12
String satCount[2];

// horizontal dilution of precision
// in the form X.X
String dilution[2];

// antenna altitude above/below mean-sea-level (geoid)
// in the form x.x
// and the associated unit, which is in meters
int antAlt[2];
String unitAlt = "M";

// Geoidal separation
// difference between the WGS-84 earth ellipsoid and 
// mean-sea-level (geoid)
// Note: "-" means mean-sea-level below ellipsoid
String geoIDSep;
String unitGeo = "M";

// age of differential GPS data,
// time in seconds since last SC104 type 1 or 9 update
// in the form x.x
//
// null field when DGPS is not used
// if diffGPS is null, then diffUsed will be false
//
// diffRefID is the reference station ID number
// ranges from 0000 to 1023
String diffGPS[2];
boolean diffUsed;
String diffRefID[4];

// checksum
String checksum[2];

// Lat/Long status
// if A, data is valid
// if V, data is invalid
String latLongStat[1];

// Selection mode for GSA, GPS, DOP, and active
// satellites
//
// returns what mode currently
String selectMode;
String currMode;

// ID of multiple satellites used for fix
String sat1ID;
String sat2ID;
String sat3ID;
String sat4ID;
String sat5ID;
String sat6ID;
String sat7ID;
String sat8ID;
String sat9ID;
String sat10ID;
String sat11ID;
String sat12ID;

// PDOP in meters
// (position dilution of precision)
String pdopM;

// HDOP in meters
// (horizontal dilution of precision)
String hdopM;

// VDOP in meters
// (vertical dilution of precision)
String vdopM;

// total number of messages when GSV 
// satellites are in view
// and its associated number
String numGSVMsg;
String msgNum;

// Satellites in view and associate number
String satInView;
String satNum;

// elevation 
String elevation;

// azimuth in degrees to true
String azimuth2True;

// SNR in dB (no idea)
String snrDB;

// Heading degrees, true
// in form x.x
String headDeg[2];

// random true boolean
boolean true;

// speed over ground in knots
// in form x.x
String groundSpeed[2];

// track made good, degrees true
// in form x.x
String trackDoneGood[2];

// date
// in form ddmmyy
String date[6];

// magnetic variation
// in degrees in form x.x
String magVar[2];

// store the new sentence here and then parse
// through to assign each of the individual 
// variables
String incomingMessage[];
int messageLength;

// allows storage of an identifier to track
// which sentence structure to use
String sentenceIdentifier[3];

// this is length 6 because the sentence ID
// consists of 6 characters:
// $--<***>, where *** is the three letter identifier
int startIDLength = 6;

// list of sentence ID's
String gpsFixData[3]=[G,G,A];
String gllGeoPos[3] = [G,L,L];
String gsaGPSDOP[3] = [G,S,A];
String gsvSatView[3] = [G,S,V];
String hdtHeading[3] = [H,D,T];
String recMinNavInfo[3] = [R,M,C];

void setup() {
  // not sure what sort of setup I'll need here yet
  // probably some sort of connection initialization
}

void loop() {
  receiveMessage();
  parseIncoming(String newMsg[]);
}

// need to figure this one out still as to how to 
// receive the message from the maretron and get it
// to the arduino via USB
void receiveMessage(){}

// takes in a new message that I received and then parses 
// through it to remove information depending upon the 
// sentence identifier
void parseIncoming(String brandNewMessage[]){
  incomingMessage[] = brandNewMessage[];
  incomingMessage.getLength() = messageLength;

  // rethink this becasue it won't work and is gross
  for(int i=0; i< startIDLength; i++){
    if(i == 0 && incomingMessage[i] == "$"){
      if(i == 1 && incomingMessage[i] == "-"){
        if(i == 2 && incomingMessage[i] == "-"){
          incomingMessage[i] = sentenceIdentifier[i-2];
        }
      }
    }
  }

  // check thorugh the sentence identifier to see which
  // parser to use
  if(sentenceIdentifier == gpsFixData){
    parseGPS();
  } else if(sentenceIdentifier == gllGeoPos){
    parseGLL();
  } else if(sentenceIdentifier == gsaGPSDOP){
    parseGSA();
  } else if(sentenceIdentifier == gsvSatView){
    parseGSV();
  } else if(sentenceIdentifier == hdtHeading){
    parseHDT();
  } else if(sentenceIdentifier == recMinNavInfo){
    parseRMC();
  } else{
    Serial.println("Something is wrong, and you never received a message");
  }
}

void parseGPS(){
  // order to parse through everything
  //hours[];
  //minutes[];
  //seconds[];
  //latitude[];
  //northCheck;
  //longitude
  // eastCheck
  // GPSQual
  // satCount[]
  // dilution
  // antAlt
  // unitAlt
  // geoIDSep
  // unitGeo
  // diffGPS
  // diffUsed
  // checksum
}

void parseGLL(){
  // latitude
  // northCheck
  // longitude
  // eastCheck
  // hours
  // minutes
  // seconds
  // latLongStat
  // checkSum
}

void parseGSA(){
  // currMode;
  // sat1ID;
  // sat2ID;
  // sat3ID;
  // sat4ID;
  // sat5ID;
  // sat6ID;
  // sat7ID;
  // sat8ID;
  // sat9ID;
  // sat10ID;
  // sat11ID;
  // sat12ID;
  // pdopM;
  // hdopM;
  // vdopM;
  // checkSum

}

void parseGSV(){
  // numGSVMsg;
  // msgNum;
  //satInView;
  // satNum;
  // elevation;
  // azimuth2True;
  // snrDB;
  // checksum
}

void parseHDT(){
  // headDeg;
  // true;
  // checkSum
}

void parseRMC(){
  // hours
  // minutes
  // seconds
  // status
  // northCheck
  // longitude
  // eastCheck
  // groundSpeed
  // trackDoneGood
  // date
  // magVar
  // checkSum
}
