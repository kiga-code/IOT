#include <SoftwareSerial.h>

SoftwareSerial bluetooth(12,11);

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
  bluetooth.begin(9600);
  Serial.begin(9600);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
}


//  Where the Magic Happens
void loop() {
  serialOutput();

  conteudo = "";
  if (bluetooth.available()) {
    conteudo = "";
    while(bluetooth.available()) {
      char caracter = bluetooth.read();

      conteudo += caracter;
      delay(10);
    }

    Serial.println("Recebido: " + conteudo);

    if (conteudo.indexOf("COMUNICACAO") >= 0) {
      bluetooth.println("{");
      bluetooth.println("SUCCESS");
      bluetooth.println("}");
    }
    if (conteudo.indexOf("ENVIAR") >= 0) {
      valor = BPM;
      Serial.println("passou");
      bluetooth.println("{");
      bluetooth.println(valor);
      bluetooth.println("}");
      
      valor += 10;
      delay(500);
    }
    conteudo = "";
  }
}
