
/* PulseSensor ™ Starter Project e Signal Tester
  * A melhor maneira de começar com, ou veja o Sinal Raw de seu PulseSensor ™ e Arduino.
  *
  * Aqui está um link para o tutorial
  * https://pulsesensor.com/pages/code-and-guide
  *
  * OBSERVE-ME (Tutorial Video):
  * https://www.youtube.com/watch?v=82T_zBZQkOE
  *
  *
-------------------------------------------------- -----------
1) Isso mostra um pulso cardíaco humano vivo.
2) Visualização ao vivo no Arduino Cool "Plotter Serial".
3) Pisque um LED em cada Heartbeat.
4) Este é o sinal do sensor de pulso direto.
5) Um excelente primeiro passo na resolução de problemas do seu circuito e conexões.
6) código "Human-legible" que é novato amigável ".


*/


//  Variables
int PulseSensorPurplePin = 0;        // Sensor de pulso PURPLE WIRE conectado a ANALOG PIN 0
int LED13 = 13;   //  O LED Arduino a bordo
int Signal;                // contém os dados brutos recebidos. O valor do sinal pode variar de 0-1024
int Threshold = 550;            // Determine qual sinal para "contar como uma batida" e qual ignorar.


// The SetUp Function:
void setup() {
  pinMode(LED13,OUTPUT);         // Pino que piscará ao seu coração!
   Serial.begin(9600);         // Configure a Comunicação Serial em determinada velocidade.

}

// A Função do Loop Principal
void loop() {

  Signal = analogRead(PulseSensorPurplePin);  // Leia o valor do PulseSensor.
                                              // Atribua esse valor à variável "Sinal".

   Serial.println(Signal);                    // Envie o valor do sinal para Plotter Serial.


   if(Signal > Threshold){                          // Se o sinal estiver acima de "550", então "on-line" o LED on-board da Arduino.
     digitalWrite(LED13,HIGH);
   } else {
     digitalWrite(LED13,LOW);                //  Além disso, o sinal deve estar abaixo de "550", então "desligue" esse LED.
   }


delay(20);


}
