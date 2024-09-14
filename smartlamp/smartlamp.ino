// Defina os pinos de LED e LDR
int ledPin = 26;          // Pino de saída para o LED
int ledValue = 10;        // Valor inicial do LED (de 0 a 100)

int ldrPin = 39;          // Pino de entrada para o LDR (sensor de luz)
int ldrMax = 4048;        // Valor máximo do LDR (dependendo do sensor)

// Configuração inicial
void setup() {
  Serial.begin(115200);   // Inicia a comunicação serial

  pinMode(ledPin, OUTPUT);  // Configura o pino do LED como saída
  pinMode(ldrPin, INPUT);   // Configura o pino do LDR como entrada

  Serial.println("SmartLamp Initialized.");
}

// Função loop que será executada continuamente pelo ESP32
void loop() {
  // Verifica se há comandos disponíveis na porta serial
  if (Serial.available() > 0) {
    // Lê o comando da serial e chama a função processCommand
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}

// Função para processar os comandos recebidos via serial
void processCommand(String command) {
  // Divide o comando em comando propriamente dito e o parâmetro
  int spaceIndex = command.indexOf(' ');
  String cmd = command.substring(0, spaceIndex);
  String param = command.substring(spaceIndex + 1);

  // Verifica e executa o comando apropriado
  if (cmd == "SET_LED") {
    int value = param.toInt();
    if (value >= 0 && value <= 100) {
      ledValue = value;
      ledUpdate(value);   // Atualiza o valor do LED
      Serial.println("RES SET_LED 1");  // Resposta de sucesso
    } else {
      Serial.println("RES SET_LED -1"); // Resposta de erro
    }
  } else if (cmd == "GET_LED") {
    Serial.printf("RES GET_LED %d\n", ledValue);  // Retorna o valor atual do LED
  } else if (cmd == "GET_LDR") {
    int ldrValue = ldrGetValue();                 // Obtém o valor do LDR
    Serial.printf("RES GET_LDR %d\n", ldrValue);  // Retorna o valor do LDR
  } else {
    Serial.println("ERR Unknown command.");       // Comando desconhecido
  }
}

// Função para atualizar o valor do LED
void ledUpdate(int value) {
  // Converte o valor recebido (de 0 a 100) para o intervalo de 0 a 255 (PWM)
  analogWrite(ledPin, map(value, 0, 100, 0, 255));
}

// Função para ler o valor do LDR
int ldrGetValue() {
  // Lê o valor do sensor LDR e o normaliza para o intervalo de 0 a 100
  int sensorValue = analogRead(ldrPin);
  return map(sensorValue, 0, ldrMax, 0, 100);
}