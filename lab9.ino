const int sensor = A0;
int sensorRead;

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
}

void loop() {
  sensorRead = analogRead(sensor);
  Serial.println(sensorRead);
}

void readInput(){
  // String outgoingString;
  // byte outgoingByte;
  // byte outgoingByte2;

  if(Serial.available() > 0){
    Serial.println("WRITING");
    // outgoingString = (String)sensor;
    // outgoingByte = (byte)sensor;
    // Send most significant byte first
    /* outgoingByte2 = outgoingByte >> 8;
    Serial.write(outgoingByte2);

    // send least significant byte second
    Serial.write(outgoingByte); */
    Serial.print(sensorRead);
  }
}
