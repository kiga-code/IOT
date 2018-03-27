


volatile int rate[10];                    // matriz para conter os últimos dez valores de IBI
volatile unsigned long sampleCounter = 0;          // usado para determinar o tempo do pulso
volatile unsigned long lastBeatTime = 0;           // usado para encontrar IBI
volatile int P =512;                      // usado para encontrar o pico na onda de pulso, semeado
volatile int T = 512;                     // usado para encontrar vale na onda de pulso, semeado
volatile int thresh = 525;                // usado para encontrar momento instantâneo de batimento cardíaco, semeado
volatile int amp = 100;                   // usado para manter a amplitude da forma de onda de pulso, semeada
volatile boolean firstBeat = true;        // usado para distribuir a matriz de taxas para que possamos inicializar com BPM razoável
volatile boolean secondBeat = false;      // usado para distribuir a matriz de taxas, então começamos com BPM razoável


void interruptSetup(){     
  // Inicializa o Timer2 para lançar uma interrupção a cada 2mS.
  TCCR2A = 0x02;    // DESATIVE PWM NOS PINOS DIGITAIS 3 E 11, E ENTRE NO MODO CTC
  TCCR2B = 0x06;     // NÃO FORÇAR COMPARAR, 256 PRESCALER
  OCR2A = 0X7C;      // AJUSTE O TOPO DA CONTAGEM PARA 124 PARA TAXA DE AMOSTRA DE 500Hz
  TIMSK2 = 0x02;     // ATIVA O INTERRUPT NA COMBINAÇÃO ENTRE O TIMER2 E OCR2A
  sei();            // CERTIFICAR-SE DE QUE AS INTERRUPÇÕES GLOBAIS SÃO PERMITIDAS     
} 


// ESTE É O TEMPORIZADOR 2 INTERRUPÇÃO DE ROTINA DE SERVIÇO.
// Timer 2 garante que façamos uma leitura a cada 2 milissegundos
ISR(TIMER2_COMPA_vect){                         // acionado quando o Timer2 conta para 124
  cli();                                      // desabilita interrupções enquanto fazemos isso
  Signal = analogRead(pulsePin);              // leia o sensor de pulso
  sampleCounter += 2;                         // acompanhar o tempo em mS com essa variável
  int N = sampleCounter - lastBeatTime;      // monitora o tempo desde a última batida para evitar ruído

    //  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3){      // evitar ruído dicrótico esperando 3/5 do último IBI
    if (Signal < T){                        // T é o cocho
      T = Signal;                         // acompanhar o ponto mais baixo da onda de pulso
    }
  }

  if(Signal > thresh && Signal > P){          // condição thresh ajuda a evitar o ruído
    P = Signal;                             // P é o pico
  }                                       // acompanhar o ponto mais alto da onda de pulso

  // AGORA É HORA DE OLHAR PARA O BATIDO DO CORAÇÃO
  // sinal aumenta em valor toda vez que há um pulso
  if (N > 250){                                   // evitar ruídos de alta frequência
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
      Pulse = true;                               // define o flag de pulso quando achamos que há um pulso
//      digitalWrite(blinkPin,HIGH);                // ligue o LED do pino 13
      IBI = sampleCounter - lastBeatTime;        // mede o tempo entre batidas em mS
      lastBeatTime = sampleCounter;               // acompanhar o tempo para o próximo pulso

      if(secondBeat){                        // se esta for a segunda batida, se secondBeat == TRUE
        secondBeat = false;                 // limpar sinalizador secondBeat
        for(int i=0; i<=9; i++){             // semeia o total em execução para obter um BPM realisitico na inicialização
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                        // se é a primeira vez que encontramos uma batida, se firstBeat == TRUE
        firstBeat = false;                   // limpar o sinalizador firstBeat
        secondBeat = true;                  // define o sinalizador de segunda batida
        sei();                               // ativar interrupções novamente
        return;                              // O valor do IBI não é confiável, portanto, descarte-o
      }   


      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                  // limpe a variável Total em execução

      for(int i=0; i<=8; i++){                // desloca dados na matriz de taxas
        rate[i] = rate[i+1];                  // e soltar o valor mais antigo do IBI
        runningTotal += rate[i];              // soma os 9 valores mais antigos do IBI
      }

      rate[9] = IBI;                          // adiciona o IBI mais recente na ordem de taxas
      runningTotal += rate[9];                // adiciona o IBI mais recente à execução do Total
      runningTotal /= 10;                     // média dos últimos 10 valores de IBI
      BPM = 60000/runningTotal;               // quantas batidas podem caber em um minuto? isso é BPM!
      QS = true;                              // definir sinalizador Self quantificado
      // QS FLAG NÃO É APAGADO DENTRO DES ISR
    }                       
  }

  if (Signal < thresh && Pulse == true){   // quando os valores estão indo para baixo, a batida acabou
//    digitalWrite(blinkPin,LOW);            // desliga o pino 13 LED
    Pulse = false;                         // redefine o enfraquecimento do pulso para que possamos fazer isso novamente
    amp = P - T;                           // obter amplitude da onda de pulso
    thresh = amp/2 + T;                    // ajusta thresh a 50% da amplitude
    P = thresh;                            // redefina isso para a próxima vez
    T = thresh;
  }

  if (N > 2500){                           // se 2,5 segundos passarem sem uma batida
    thresh = 512;                          // define o padrão do thresh 
    P = 512;                               // define o padrão do P
    T = 512;                               // define o padrão do T
    lastBeatTime = sampleCounter;          // traz o último tempo de batida atualizado      
    firstBeat = true;                      // define estes para evitar o ruído
    secondBeat = false;                   // quando tivermos o batimento cardíaco de volta
  }

  sei();                                   // habilitar interrupções quando terminar!
}





