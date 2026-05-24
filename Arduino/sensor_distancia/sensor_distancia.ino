// Definição dos pinos do sensor ultrassônico HC-SR04
const int PIN_TRIG = 9;
const int PIN_ECHO = 10;

char charRead;
int distanciaAnterior = -1;
bool ensaioAtivo = false; // Controle de fluxo de transmissão

void setup() {
  Serial.begin(9600);
  
  // Configuração das portas do transdutor
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  // Assegura que o pino de disparo inicie em repouso
  digitalWrite(PIN_TRIG, LOW);
}

void loop() {
  // 1. Escuta de sinais de controle do servidor Node.js
  if (Serial.available() > 0) { 
    charRead = Serial.read(); 
    
    if (charRead == 'T') {
      ensaioAtivo = true;  // Inicia a varredura instantaneamente
    } else if (charRead == 'F') {
      ensaioAtivo = false; // Suspende a varredura
    } 
  }

  // 2. Aquisição e processamento do sinal ultrassônico
  if (ensaioAtivo) {
    
    // Disparo do pulso de 40kHz (10 microssegundos exatos)
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH); 
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    // Janela de recepção (timeout restrito a 20ms para viabilizar amostragem a 10Hz)
    long duracao = pulseIn(PIN_ECHO, HIGH, 20000); 
    int distanciaAtual = (duracao * 0.0343) / 2;
    
    // Tratamento de falha de eco (ausência de retorno no tempo limite)
    if (distanciaAtual == 0 && duracao == 0) {
      if (distanciaAnterior != -1) {
        distanciaAtual = distanciaAnterior;
      }
    }

    // Filtro de rejeição de ruído para anomalias espaciais (>30cm de variação em 0.1s)
    if (distanciaAnterior != -1) {
      int delta = abs(distanciaAtual - distanciaAnterior);
      if (delta > 30) { 
        distanciaAtual = distanciaAnterior; 
      }
    }
    
    distanciaAnterior = distanciaAtual;
    
    // Transmissão isolada do valor numérico com delimitador explícito de quebra de linha
    Serial.print(distanciaAtual); 
    Serial.print("\r\n");
    
    // Período de amostragem (500ms)
    delay(500); 
  }
}
