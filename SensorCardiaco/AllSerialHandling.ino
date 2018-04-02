
//////////
/////////  All Serial Handling Code,
/////////  It's Changeable with the 'serialVisual' variable
/////////  Set it to 'true' or 'false' when it's declared at start of code.
/////////

void serialOutput() {  // Decide How To Output Serial.
  if (serialVisual == true) {
    arduinoSerialMonitorVisual('-', Signal);   // goes to function that makes Serial Monitor Visualizer
  } else {
    sendDataToSerial('S', Signal);     // goes to sendDataToSerial function
  }
}


//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens() {
  if (serialVisual == true) {
    String json = "{\"bpm\":";
    json += BPM;
    json += "}";
    return BPM;
    
  } else {
    sendDataToSerial('B', BPM);
    sendDataToSerial('Q', IBI);
  }
}




void sendDataToSerial(char symbol, int data ) {
  Serial.print(symbol);

  Serial.println(data);
}





void ledFadeToBeat() {
  fadeRate -= 15;
  fadeRate = constrain(fadeRate, 0, 255);
  analogWrite(fadePin, fadeRate);
}


//  Code to Make the Serial Monitor Visualizer Work
void arduinoSerialMonitorVisual(char symbol, int data ) {
  const int sensorMin = 0;
  const int sensorMax = 1024;

  int sensorReading = data;

  int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
}


