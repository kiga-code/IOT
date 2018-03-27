
void serialOutput() { // Decide como a saída serial.
  if (serialVisual == true) {
    arduinoSerialMonitorVisual('-', Signal);   // vai funcionar que torna o Serial Monitor Visualizer
  } else {
    sendDataToSerial('S', Signal);     // vai para a função enviando Dados para a Serial
  }
}


// Decide como sair de dados BPM e IBI
void serialOutputWhenBeatHappens() {
  if (serialVisual == true) {
    Serial.print("*** Heart-Beat Happened *** ");
    Serial.print("BPM: ");
    String json = "{\"bpm\":";
    json += BPM;
    json += "}";
    Genotronex.println(json);
    Serial.print(BPM);
    Serial.println();
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


// Código para fazer o Visualizador do Monitor Serial funcionar
void arduinoSerialMonitorVisual(char symbol, int data ) {
  const int sensorMin = 0;
  const int sensorMax = 1024;

  int sensorReading = data;

  int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
}


