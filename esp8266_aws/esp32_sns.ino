#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Defina as credenciais WiFi
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASSWORD"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* apiUrl = "URL_SEND_SNS";

WiFiClient wifiClient;
HTTPClient httpClient;

void setup() {
  Serial.begin(9600);  // Ajuste a taxa de transmissão serial para 9600
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readString();
    Serial.print("Publishing message: ");
    Serial.println(message);

    if (WiFi.status() == WL_CONNECTED) {
      httpClient.begin(wifiClient, apiUrl);
      httpClient.addHeader("Content-Type", "application/json");

      String payload = "{\"message\": \"" + message + "\"}";
      int httpResponseCode = httpClient.POST(payload);

      if (httpResponseCode > 0) {
        String response = httpClient.getString();
        Serial.println("Response: " + response);
      } else {
        Serial.printf("Error on HTTP request: %s\n", httpClient.errorToString(httpResponseCode).c_str());
      }

      httpClient.end();
    } else {
      Serial.println("Error in WiFi connection");
    }
  }

  delay(1000); // Aguarde um segundo antes de verificar novamente
}
