int encoderPin = 38;
int n = LOW;
int encoderPos = 0;
int encoderPinLast = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(encoderPin, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  n = digitalRead(encoderPin);
  if ((encoderPinLast == LOW) && (n == HIGH)) {
      encoderPos--;
    } else {
      encoderPos++;
    }
    Serial.println (encoderPos);
    encoderPinLast = n;
 }
