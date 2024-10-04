#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <MFRC522.h>
 
#define RST_PIN     D0
#define SS_PIN      D8
 
MFRC522 mfrc522(SS_PIN, RST_PIN);
 
const char* ssid = "TamilZorous";
const char* password = "TZ@12477";
const char* apiEndpoint = "https://api-generator.retool.com/PU6wzU/visitor_pass/1";
String status = "-1"; // Changed status to String type
 
void sendPostRequest(String tagUID = "");
 
void setup() {
  Serial.begin(115200);
  delay(1000);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
 
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID reader initialized");
 
  sendPostRequest();
}
 
void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      tagUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      tagUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    tagUID.toUpperCase();
    sendPostRequest(tagUID);
    delay(1000);
  }
}
 
void sendPostRequest(String tagUID) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
 
  Serial.print("RFID Tag UID: ");
  Serial.println(tagUID);
 
  // Construct JSON document
  StaticJsonDocument<512> jsonDoc;
 
  // Populate JSON document based on status
  if (status == "-1") {
    // If the status is "-1", set initial data
    jsonDoc["id"] = 1;
    jsonDoc["name"] = "Brigid Blackmoor";
    jsonDoc["pass"] = "149529828404753";
    jsonDoc["type"] = "Cooking";
    jsonDoc["email"] = "ceilles29@google.ru";
    jsonDoc["mobile"] = "(555) 852-9178";
    jsonDoc["status"] = "NOT_YET_REACHED";
    jsonDoc["to_date"] = "May 26, 2024 10:24 PM"; // ISO 8601 format
    jsonDoc["from_date"] = "Jun 1, 2024 9:03 PM"; // ISO 8601 format
    jsonDoc["entry_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    jsonDoc["exit_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    status = "0";
  } else if (status == "0") {
    // If status is "0", set entry data
    jsonDoc["id"] = 1;
    jsonDoc["name"] = "Brigid Blackmoor";
    jsonDoc["pass"] = "149529828404753";
    jsonDoc["type"] = "Cooking";
    jsonDoc["email"] = "ceilles29@google.ru";
    jsonDoc["mobile"] = "(555) 852-9178";
    jsonDoc["status"] = "ENTRY";
    jsonDoc["to_date"] = "May 26, 2024 10:24 PM"; // ISO 8601 format
    jsonDoc["from_date"] = "Jun 1, 2024 9:03 PM"; // ISO 8601 format
    jsonDoc["entry_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    jsonDoc["exit_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    status = "1";
  } else if (status == "1") {
    // If status is "1", set exit data
    jsonDoc["id"] = 1;
    jsonDoc["name"] = "Brigid Blackmoor";
    jsonDoc["pass"] = "149529828404753";
    jsonDoc["type"] = "Cooking";
    jsonDoc["email"] = "ceilles29@google.ru";
    jsonDoc["mobile"] = "(555) 852-9178";
    jsonDoc["status"] = "EXIT";
    jsonDoc["to_date"] = "May 26, 2024 10:24 PM"; // ISO 8601 format
    jsonDoc["from_date"] = "Jun 1, 2024 9:03 PM"; // ISO 8601 format
    jsonDoc["entry_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    jsonDoc["exit_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    status = "2";
  } else {
    // If status is not recognized, set invalid data and reset status
    jsonDoc["id"] = 1;
    jsonDoc["name"] = "Brigid Blackmoor";
    jsonDoc["pass"] = "149529828404753";
    jsonDoc["type"] = "Cooking";
    jsonDoc["email"] = "ceilles29@google.ru";
    jsonDoc["mobile"] = "(555) 852-9178";
    jsonDoc["status"] = "IN_VALID";
    jsonDoc["to_date"] = "May 26, 2024 10:24 PM"; // ISO 8601 format
    jsonDoc["from_date"] = "Jun 1, 2024 9:03 PM"; // ISO 8601 format
    jsonDoc["entry_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    jsonDoc["exit_at"] = "2024-01-01T00:00:00"; // ISO 8601 format
    status = "0"; // Reset status to default
  }
 
  // Convert JSON document to string
  String requestBody;
  serializeJson(jsonDoc, requestBody);
 
  Serial.print("Sending POST request to: ");
  Serial.println(apiEndpoint);
  Serial.print("Request Body: ");
  Serial.println(requestBody);
 
  // Send HTTP request
  http.begin(client, apiEndpoint);
  http.addHeader("Content-Type", "application/json");
 
  int httpResponseCode = http.PUT(requestBody);
 
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println("Response: ");
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
 
  http.end();
}
 
has context menu