#include <SoftwareSerial.h>

SoftwareSerial bluetooth(12, 11);
int ledpin = 12;

//  Variables
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
String conteudo;
int valor = 0;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse


void setup() {
  pinMode(blinkPin, OUTPUT);
  pinMode(fadePin, OUTPUT);
  Serial.begin(115200);
  interruptSetup();

  bluetooth.begin(9600);
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  Serial.println("Inicio");
}

//  Where the Magic Happens
void loop() {
  serialOutput();

  if (QS == true) {    // Foi encontrada uma pulsação
    digitalWrite(blinkPin, HIGH);
    fadeRate = 255;
    serialOutputWhenBeatHappens();
    QS = false;
  }
  else {
    digitalWrite(blinkPin, LOW);
  }
  delay(20);
}
