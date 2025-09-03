#include <WiFi.h>

// Substitua pelo seu Wi-Fi
const char* ssid = "";
const char* password = "";

const int led = 8; // LED interno
WiFiServer server(80);

bool piscar = false; // Controle do modo piscar
bool estadoLed = false; // Estado atual do LED
unsigned long previousMillis = 0;
const long interval = 200; // Intervalo para piscar rápido (ms)

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.begin(115200);
  addLog("Conectando ao Wi-Fi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    addLog(".");
  }

  addLog("");
  addLog("Wi-Fi conectado!");
  addLog("IP do ESP32: " + WiFi.localIP().toString());

  server.begin();
}

void loop() {
  // Controle do piscar
  if (piscar) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      digitalWrite(led, !digitalRead(led)); // alterna o estado do LED
    }
  }

  WiFiClient client = server.available();
  if (client) {
    addLog("Novo cliente conectado!");
    String request = client.readStringUntil('\r');
    addLog("Requisição: " + request);
    client.flush();

    // Controle do LED
    if (request.indexOf("/LED=ON") != -1) {
      estadoLed = true;
      piscar = false;
      digitalWrite(led, HIGH);
      addLog("LED LIGADO");
    }
    if (request.indexOf("/LED=OFF") != -1) {
      estadoLed = false;
      piscar = false;
      digitalWrite(led, LOW);
      addLog("LED DESLIGADO");
    }
    if (request.indexOf("/LED=BLINK") != -1) {
      piscar = true;
      addLog("LED PISCANDO RÁPIDO");
    }
    if (request.indexOf("/LED=STOPBLINK") != -1) {
      piscar = false;
      estadoLed = false;
      digitalWrite(led, LOW);
      addLog("LED PAROU DE PISCAR");
    }

    // Página web responsiva com log
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=UTF-8");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>");
    client.println("body { font-family: Arial; text-align: center; padding: 20px; }");
    client.println("button { padding: 20px; font-size: 20px; margin: 10px; width: 80%; max-width: 300px; }");
    client.println(".log { text-align: left; margin-top: 20px; padding: 10px; border: 1px solid #ccc; max-height: 200px; overflow-y: scroll; background: #f9f9f9; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>Controle do LED ESP32-C3</h1>");

    // Botões corrigidos
    client.println("<a href=\"/LED=ON\"><button>DESLIGAR</button></a>");
    client.println("<a href=\"/LED=OFF\"><button>LIGAR</button></a>");
    client.println("<a href=\"/LED=BLINK\"><button>PISCAR RÁPIDO</button></a>");
    client.println("<a href=\"/LED=STOPBLINK\"><button>PARAR DE PISCAR</button></a>");

    client.stop();
    addLog("Cliente desconectado.");
  }
}
