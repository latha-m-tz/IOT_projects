#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char*  ssid = "TamilZorous";
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
    Serial.println("Making HTTP GET request...");
    
    HTTPClient http;
    WiFiClient client;
  
    if (http.begin(client,"http://jsonplaceholder.typicode.com/posts")) { // Specify the URL
      int httpCode = http.GET(); // Make the GET request
      
      if (httpCode > 0) { // Check for successful response
        String payload = http.getString(); // Get the response payload
        
        // Print the response payload
        Serial.println("Response:");
        Serial.println(payload);

        DynamicJsonDocument doc(1024); // Specify the size of the JSON document
        DeserializationError error = deserializeJson(doc, payload);

  // Check for parsing errors
  if (error) {
    Serial.print("Error parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract values from the JSON document
  JsonObject result = doc["result"][0]; // Get the first item in the "result" array
  int id = result["id"];
  const char* name = result["name"];
  const char* address = result["address"];

  // Print the extracted values
  Serial.print("ID: ");
  Serial.println(id);
  // Serial.print("Name: ");
  // Serial.println(name);
  // Serial.print("Address: ");
  // Serial.println(address);
  
      Serial.println("\n");
       
      } else {
        Serial.print("HTTP GET request failed, error: ");
        Serial.println(httpCode);
      }

      http.end(); // Close connection
    } else {
      Serial.println("Unable to connect to server");
    }
  } else {
    Serial.println("WiFi disconnected,   reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconnected");

  }
  
  delay(5000); // Wait for 5 seconds before making the next request
}
