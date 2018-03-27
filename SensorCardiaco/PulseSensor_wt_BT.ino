
/*

  ----------------------  Notes ----------------------  ----------------------
  Este código:
  1) Pisca um LED no PIN do Heartbeat Live do usuário 13
  2) Fadeia um LED para HeartBeat Live do usuário
  3) Determina BPM
  4) imprime todos os acima para serial


*/
//bluetooth
#include <SoftwareSerial.h>//importar a biblioteca serial

SoftwareSerial Genotronex(10, 11); // RX, TX
int ledpin = 12; // led em D13 mostrará piscando on / off
int BluetoothData; // os dados fornecidos pelo Computer

//  Variables
int pulsePin = 0;                 // Fio roxo de sensor de pulso conectado ao pino analógico 0
int blinkPin = 13;                // pino ao piscar led a cada batida
int fadePin = 5;                  // pino para fazer o desbotamento de classe elegante piscar a cada batida
int fadeRate = 0;                 // usado para diminuir o LED com PWM no fadePin


// Volatile, usado na rotina de serviço de interrupção!

volatile int BPM;                   // int que mantém o Analog in 0. atualizado a cada 2mS
volatile int Signal;                // mantém os dados brutos recebidos
volatile int IBI = 600;             // int que contém o intervalo de tempo entre as batidas!
volatile boolean Pulse = false;     // "True" quando a pulsação ao vivo do usuário é detectada. "Falso" quando não é uma "batida ao vivo".
volatile boolean QS = false;        // se torna verdade quando Arduoino encontra uma batida.

// Considera a saída serial - Configure isso de acordo com suas necessidades
static boolean serialVisual = true;   // Defina como 'false' por padrão. Reinicie para 'true' para ver o Monitor Serial do Arduino ASCII Visual Pulse


void setup() {
  pinMode(blinkPin, OUTPUT);        // pino que piscará ao seu batimento cardíaco!
  pinMode(fadePin, OUTPUT);         // Pino que irá desaparecer ao seu batimento cardíaco!
  Serial.begin(115200);             // nós concordamos em falar rápido!
  interruptSetup();                // configura para ler o sinal do sensor de pulso a cada 2mS



  Genotronex.begin(9600);
  Genotronex.println("Bluetooth On por favor, pressione 1 ou 0 LED pisca ..");
  pinMode(ledpin, OUTPUT);
  Serial.println("Inicio");
}


void loop() {

  serialOutput() ;

  if (QS == true) {    // Foi encontrada uma pulsação
    // BPM e IBI foram determinados
    // Quantified Self "QS" true quando o arduino encontra um batimento
    digitalWrite(blinkPin, HIGH);    // Blink LED, we got a beat.
    fadeRate = 255;         // Faz com que o efeito de desvanecimento do LED aconteça
    // Configura a variável 'fadeRate' para 255 para diminuir o brilho do LED
    serialOutputWhenBeatHappens();   //Saída para serial.
    QS = false;                      // redefinir o sinalizador auto-quantificado para a próxima vez
  }
  else {

    digitalWrite(blinkPin, LOW);           // Não há batida, desligue o pino 13 LED
  }

  ledFadeToBeat();                      // Faz com que o efeito de enfraquecendo da LED aconteça
  if (Genotronex.available()) {
    BluetoothData = Genotronex.read();
    if (BluetoothData == '1') { // se o número 1 for pressionado ....
      digitalWrite(ledpin, 1);
      Genotronex.println("LED  On D12 ON ! ");
    }
    if (BluetoothData == '0') { // se o número 0 for pressionado ....
      digitalWrite(ledpin, 0);
      Genotronex.println("LED  On D12 Off ! ");
    }
  }
  delay(20);           //dar um intervalo
}









