#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
 
const char* ssid = "TZ Training Institute";
const char* password = "TZ@TZ@124$";
const char* apiEndpoint = "http://apiabtex.tamilzorous.com/api/switch_on_off"; // Updated API endpoint
 
const int ledPin = 2; //D4
 
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with the LED off
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
 
    http.begin(client, apiEndpoint);
 
    http.addHeader("Content-Type", "application/json");
 
    StaticJsonDocument<200> doc;
    doc["request"] = "status"; // or whatever data you need to send
    String payload;
    serializeJson(doc, payload);
 
    int httpCode = http.POST(payload);
 
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println(response);
 
      StaticJsonDocument<200> responseDoc;
      DeserializationError error = deserializeJson(responseDoc, response);
 
      if (!error) {
        const char* ledStatus = responseDoc["led_status"];
 
        if (strcmp(ledStatus, "ON") == 0) {
          digitalWrite(ledPin, HIGH);
        } else {
          digitalWrite(ledPin, LOW);
        }
      } else {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("Error in HTTP request, code: ");
      Serial.println(httpCode);
    }
  
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
 
  delay(300000); // Wait before making another request
}
 
 