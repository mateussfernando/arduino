#include "WiFi.h"

const char* ssid = "teste";

// Intervalo de atualização do log
unsigned long previousMillis = 0;
const long interval = 20000;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  Serial.println("Ponto de WIFI Iinicado!");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("IP do AP: "); Serial.println(WiFi.softAPIP());
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("-------------------------");
  }
}
