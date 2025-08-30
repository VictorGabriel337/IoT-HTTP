#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- Config WiFi ---
const char* ssid = "Aimee1306";  
const char* password = "@fernandacampos1306";  

// --- Servidor HTTP ---
ESP8266WebServer server(80);

// --- LED ---
const int ledPin = LED_BUILTIN;

// --- Funções de rota ---
void handleNotFound() {
  server.send(404, "application/json", "{\"message\":\"Not Found\"}");
}

void handleLedOn() {
  digitalWrite(ledPin, LOW); // No ESP8266, LOW = LED aceso
  server.send(200, "application/json", "{\"led\":\"on\"}");
  Serial.println("LED ligado ✅");
}

void handleLedOff() {
  digitalWrite(ledPin, HIGH); // HIGH = LED apagado
  server.send(200, "application/json", "{\"led\":\"off\"}");
  Serial.println("LED desligado ❌");
}

// --- Conectar ao WiFi ---
void connectWiFi() {
  Serial.println("🔌 Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;

    if (tentativas > 30) { // ~15 segundos
      Serial.println("\n⚠️ Falha ao conectar. Reiniciando...");
      ESP.restart();
    }
  }

  Serial.println("\n✅ WiFi conectado!");
  Serial.print("IP do ESP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(1000); // estabiliza Serial
  Serial.println("🚀 Iniciando ESP8266...");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LED começa apagado

  // 1) Conectar primeiro ao WiFi
  connectWiFi();

  // 2) Só depois configurar servidor HTTP
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("🌐 Servidor HTTP iniciado!");
  Serial.println("Use /led/on ou /led/off no navegador.");
}

void loop() {
  server.handleClient(); // Mantém servidor rodando
}
