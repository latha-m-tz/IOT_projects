#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <NewPing.h>
 
// Network credentials
const char* ssid = "TZ Training Institute";
const char* password = "TZ@TZ@124$";
 
// API endpoint
const char* apiEndpoint = "http://apiabtex.tamilzorous.com/api/multiple_switch_on_off";
 
// Define pins for buttons, LEDs, and ultrasonic sensor
#define BUTTON_PIN1 D5
#define LED_PIN1    D3
#define LED_PIN2    D4
#define TRIGGER_PIN  D0
#define ECHO_PIN     D1
 
const int maxDistance = 200; // Maximum distance (in cm) for ultrasonic sensor
 
int buttonState1 = HIGH;
 
bool ledState1 = false;
bool ledState2 = false;
 
// Variables for debouncing
unsigned long lastDebounceTime1 = 0;
unsigned long debounceDelay = 50;
 
// Variables for button states
bool lastButtonState1 = HIGH;
 
// Retry settings
const unsigned long retryDelay = 200; // 2 seconds
 
// Create NewPing object
NewPing sonar(TRIGGER_PIN, ECHO_PIN, maxDistance);
 
// Global variable for distance
float distanceCm = 0;
float lastDistanceCm = 0; // To track the last distance measurement
 
// Timestamp for the last update
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 2000; // Update interval (100 milliseconds)
 
void setup() {
  // Initialize LED pins as outputs
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
 
  // Initialize button pins as inputs with internal pull-up resistors
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
 
  // Start the Serial Monitor
  Serial.begin(115200);
 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected to WiFi");
} 
 
void loop() {
  // Read button states
  int reading1 = digitalRead(BUTTON_PIN1);
 
  // Handle button 1
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == LOW) {
        ledState1 = !ledState1; // Toggle LED state
        digitalWrite(LED_PIN1, ledState1 ? HIGH : LOW);
        sendStatusUpdate(); // Send status update on button press
      }
    }
  }
  lastButtonState1 = reading1;
 
  // Handle ultrasonic sensor with reduced frequency
  static unsigned long lastSensorReadTime = 0;
  const unsigned long sensorReadInterval = 500; // Read sensor every 5 seconds
  if (millis() - lastSensorReadTime >= sensorReadInterval) {
    handleUltrasonicSensor();
    lastSensorReadTime = millis(); // Update the last sensor read time
  }
 
  // Update status if necessary
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis(); // Update timestamp
  }
 
  delay(100); // Short delay to avoid rapid polling
}
 
void handleUltrasonicSensor() {
  unsigned int distance = sonar.ping_cm();
  if (distance == 0) {
    distanceCm = maxDistance; // Handle out-of-range values
  } else {
    distanceCm = distance;
  }
 
  if (distanceCm != lastDistanceCm) {
    // Only update if the distance has changed
    if (distanceCm <  20) {
      if (!ledState2) {
        ledState2 = true; // Turn on LED2 if it's off
        digitalWrite(LED_PIN2, HIGH);
        sendStatusUpdate(); // Send status update on distance change
      }
    } else {
      if (ledState2) {
        ledState2 = false; // Turn off LED2 if it's on
        digitalWrite(LED_PIN2, LOW);
        sendStatusUpdate(); // Send status update on distance change
      }
    }
 
    // Print distance to Serial Monitor
    // Serial.print("Distance (cm): ");
    // Serial.println(distanceCm);
 
    // Update lastDistanceCm
    lastDistanceCm = distanceCm;
  }
}
 
void sendStatusUpdate() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, apiEndpoint);
    http.addHeader("Content-Type", "application/json");
 
    // Create JSON payload
    StaticJsonDocument<400> jsonPayload;
    JsonArray switches = jsonPayload.createNestedArray("switch");
 
      Serial.println(switches);
    // Switch 1 (Pushbutton with LED1)
    JsonObject switch1 = switches.createNestedObject();
    switch1["switch"] = "1";
    switch1["led_status"] = ledState1 ? "ON" : "OFF";
    switch1["sensor"] = 1.1; // Pushbutton does not have a sensor
 
    // Switch 2 (Sensor with LED2)
    JsonObject switch2 = switches.createNestedObject();
    switch2["switch"] = "2";
    switch2["led_status"] = ledState2 ? "ON" : "OFF";
     switch2["sensor"] =5.6; //(distanceCm < 20) ? distanceCm : distanceCm; // Send sensor value or "null"
 
    // Debugging: Print the sensor value for Switch 2
    Serial.print("Sending Sensor Value for Switch 2: ");
    Serial.println(distanceCm);
 
    // Switch 3 (No hardware, so set null values)
    JsonObject switch3 = switches.createNestedObject();
    switch3["switch"] = "3";
    switch3["led_status"] = "OFF"; // No hardware for switch 3
    switch3["sensor"] = 123.05;     // No hardware for switch 3
 
    String payload;
    serializeJson(jsonPayload, payload);
 
    Serial.println("REQUEST: " + payload);
    int httpCode = http.POST(payload);
 
    String response = http.getString();
    Serial.println("HTTP Code: " + String(httpCode));
    Serial.println("Response: " + response);
 
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Status update successful.");
    } else if (httpCode == HTTP_CODE_TOO_MANY_REQUESTS) {
      Serial.println("Rate limit exceeded. Retrying later.");
      delay(retryDelay);
    } else {
      // Handle other errors
      Serial.println("Error occurred. HTTP Code: " + String(httpCode));
    }
 
    http.end();
  } else {
    Serial.println("WiFi not connected. Check connection.");
  }
}
 