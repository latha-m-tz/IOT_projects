#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TamilZorous";
const char* password = "TZ@12477";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Connecting to WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Making HTTP POST request...");
    
    HTTPClient http;
    WiFiClient client;
    
    if (http.begin(client, "http://apiabtex.tamilzorous.com/api/store")) { // Specify the URL
      http.addHeader("Content-Type", "application/json");
      //http.addHeader("Authorization", auth);

      // Create JSON payload
      DynamicJsonDocument jsonDoc(1024);
      jsonDoc["address"] = "123 Main St";
      //jsonDoc["phone"] = "+1234567890";
      //jsonDoc["company"] = "ABC Inc";
      jsonDoc["name"] = "latha";
      //jsonDoc["switch_state"] = "1";
      
      String requestBody;
      serializeJson(jsonDoc, requestBody);

      int httpCode = http.POST(requestBody); // Make the POST request
      
      if (httpCode > 0) { // Check for successful response
        String payload = http.getString(); // Get the response payload
        
        // Print the response payload
        Serial.println("Response:");
        Serial.println(payload);
      } else {
        Serial.print("HTTP POST request failed, error: ");
        Serial.println(httpCode);
      }

      http.end(); // Close connection
    } else {
      Serial.println("Unable to connect to server");
    }
  } else {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);//     
      Serial.print(".");
    }
    Serial.println("WiFi reconnected");
  }
  
  delay(5000); // Wait for 5 seconds before making the next request
}
