#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
 
// WiFi credentials
const char* ssid = "TamilZorous";
const char* password = "TZ@12477";
 
// API endpoint
const char* apiEndpoint = "https://api-generator.retool.com/PU6wzU/visitor_pass/1";
 
// Timezone offset in seconds (19800 seconds = UTC+5:30)
const long utcOffsetInSeconds = 19800;
 
// NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
 
// RFID pins
#define RST_PIN D0
#define SS_PIN D8
 
MFRC522 mfrc522(SS_PIN, RST_PIN);
 
// Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
 
String status = "0"; // Changed status to String type
String entry_time = "";
long timestamp_temp = 0;
 
// Function prototypes
void sendPostRequest(String tagUID = "");
long time_stamp_();
String datetime_();
 
void setup() {
  Serial.begin(115200);
 
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
 
  // Initialize NTPClient
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
 
  // Initialize RFID reader
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID reader initialized");
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
 
  // Construct JSON document
  StaticJsonDocument<512> jsonDoc;
 
  if (status == "0") {
    // If status is "0", set entry data
    jsonDoc["id"] = 1;
    jsonDoc["name"] = "Brigid Blackmoor";
    jsonDoc["pass"] = "149529828404753";
    jsonDoc["type"] = "Cooking";
    jsonDoc["email"] = "ceilles29@google.ru";
    jsonDoc["mobile"] = "(555) 852-9178";
    jsonDoc["status"] = "ENTRY";
    jsonDoc["to_date"] = ""; // ISO 8601 format
    jsonDoc["from_date"] = datetime_();
    jsonDoc["entry_at"] = time_stamp_();
    jsonDoc["exit_at"] = ""; // ISO 8601 format
    status = "1";
    entry_time = jsonDoc["from_date"].as<String>();
    timestamp_temp = jsonDoc["entry_at"];
  } else if (status == "1") {
    // If status is "1", set exit data
    jsonDoc["id"] = 1;
    jsonDoc["name"] = "Brigid Blackmoor";
    jsonDoc["pass"] = "149529828404753";
    jsonDoc["type"] = "Cooking";
    jsonDoc["email"] = "ceilles29@google.ru";
    jsonDoc["mobile"] = "(555) 852-9178";
    jsonDoc["status"] = "EXIT";
    jsonDoc["to_date"] = datetime_();
    jsonDoc["from_date"] = entry_time;
    jsonDoc["entry_at"] = timestamp_temp;
    jsonDoc["exit_at"] = time_stamp_();
    status = "0";
  }
 
  // Convert JSON document to string
  String requestBody;
  serializeJson(jsonDoc, requestBody);
 
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
 
long time_stamp_() {
  timeClient.update();
  // Get time components
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
 
  // Convert 24-hour format to 12-hour format
  int hours12 = hours % 12;
  if (hours12 == 0) {
    hours12 = 12; // 0 should be displayed as 12 in 12-hour format
  }
 
  // Determine AM or PM
  String period = (hours < 12) ? "AM" : "PM";
 
  // Add leading zeros if needed
  String hoursStr = (hours12 < 10) ? "0" + String(hours12) : String(hours12);
  String minutesStr = (minutes < 10) ? "0" + String(minutes) : String(minutes);
  String secondsStr = (seconds < 10) ? "0" + String(seconds) : String(seconds);
 
  // Construct time string in AM/PM format
  String currentTime = hoursStr + ":" + minutesStr + ":" + secondsStr + " " + period;
 
  time_t epochTime = timeClient.getEpochTime();
 
  // Get a time structure
  struct tm *ptm = gmtime(&epochTime);
 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon;
  String currentMonthName = months[currentMonth];
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(currentMonthName) + " " + String(monthDay) + ", " + String(currentYear);
 
  // Concatenate time and date
  String dateTime = currentDate + " " + currentTime;
 
  // Convert dateTime to timestamp in milliseconds
  struct tm timeinfo;
  timeinfo.tm_year = currentYear - 1900;
  timeinfo.tm_mon = currentMonth;
  timeinfo.tm_mday = monthDay;
  timeinfo.tm_hour = hours;
  timeinfo.tm_min = minutes;
  timeinfo.tm_sec = seconds;
  time_t timestamp = mktime(&timeinfo);
  long final_timestamp = timestamp - 19800;
 
  return final_timestamp;
}
 
String datetime_() {
  timeClient.update();
 
  // Get time components
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
 
  // Convert 24-hour format to 12-hour format
  int hours12 = hours % 12;
  if (hours12 == 0) {
    hours12 = 12; // 0 should be displayed as 12 in 12-hour format
  }
 
  // Determine AM or PM
  String period = (hours < 12) ? "AM" : "PM";
 
  // Add leading zeros if needed
  String hoursStr = (hours12 < 10) ? "0" + String(hours12) : String(hours12);
  String minutesStr = (minutes < 10) ? "0" + String(minutes) : String(minutes);
  String secondsStr = (seconds < 10) ? "0" + String(seconds) : String(seconds);
 
  // Construct time string in AM/PM format
  String currentTime = hoursStr + ":" + minutesStr + " " + period;
 
  time_t epochTime = timeClient.getEpochTime();
 
  // Get a time structure
  struct tm *ptm = gmtime(&epochTime);
 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon;
  String currentMonthName = months[currentMonth];
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(currentMonthName) + " " + String(monthDay) + ", " + String(currentYear);
 
  // Concatenate time and date
  String dateTime = currentDate + " " + currentTime;
 
  return dateTime;
}
 
 