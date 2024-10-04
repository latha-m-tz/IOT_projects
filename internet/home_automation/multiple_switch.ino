// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// #include <ArduinoJson.h>

// const char* ssid = "FTTH";
// const char* password = "12345678";
// const char* apiEndpoint = "http://apiabtex.tamilzorous.com/api/multiple_switch_on_off";

// #define BUTTON_PIN1 D5
// #define BUTTON_PIN2 D6
// #define BUTTON_PIN3 D7  
// #define LED_PIN1    D3
// #define LED_PIN2    D2
// #define LED_PIN3    D4

// String buttonState1 = ""; 
// String buttonState2 = "";
// String buttonState3 = "";

// bool ledState1 = false;
// bool ledState2 = false;
// bool ledState3 = false;

// void setup() {
//     pinMode(LED_PIN1, OUTPUT);
//     pinMode(LED_PIN2, OUTPUT);
//     pinMode(LED_PIN3, OUTPUT);

//     pinMode(BUTTON_PIN1, INPUT_PULLUP);
//     pinMode(BUTTON_PIN2, INPUT_PULLUP);
//     pinMode(BUTTON_PIN3, INPUT_PULLUP);

//     Serial.begin(115200);

//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//     }
//     Serial.println("Connected to WiFi");
// }

// void loop() {
//     string buttonState1  = digitalRead(BUTTON_PIN1);
//     string buttonState2  = digitalRead(BUTTON_PIN2);
//     string buttonState3  = digitalRead(BUTTON_PIN3);

//     if (buttonState1 != String(reading1)) {
//         buttonState1 = String(reading1);
//         if (buttonState1 == "1") {
//             ledState1 = true;
//             digitalWrite(LED_PIN1, HIGH);
//         } else { 
//             ledState1 = false;
//             digitalWrite(LED_PIN1, LOW);
//         }
//         sendStatusUpdate();
//     }

//     if (buttonState2 != String(reading2)) {
//         buttonState2 = String(reading2);
//         if (buttonState2 == "1") {
//             ledState2 = true;
//             digitalWrite(LED_PIN2, HIGH);
//         } else {
//             ledState2 = false;
//             digitalWrite(LED_PIN2, LOW);
//         }
//         sendStatusUpdate();
//     }

//     if (buttonState3 != String(reading3)) {
//         buttonState3 = String(reading3);
//         if (buttonState3 == "1") {
//             ledState3 = true;
//             digitalWrite(LED_PIN3, HIGH);
//         } else {
//             ledState3 = false;
//             digitalWrite(LED_PIN3, LOW);
//         }
//         sendStatusUpdate();
//     }
// }

// void sendStatusUpdate() {
//     if (WiFi.status() == WL_CONNECTED) {
//         WiFiClient client;
//         HTTPClient http;
//         http.begin(client, apiEndpoint);
//         http.addHeader("Content-Type", "application/json");

//         StaticJsonDocument<300> jsonPayload;
//         JsonArray switches = jsonPayload.createNestedArray("switch");

//         JsonObject switch1 = switches.createNestedObject();
//         switch1["switch"] = "1";
//         switch1["led_status"] = ledState1 ? "ON" : "OFF";

//         JsonObject switch2 = switches.createNestedObject();
//         switch2["switch"] = "2";
//         switch2["led_status"] = ledState2 ? "ON" : "OFF";

//         JsonObject switch3 = switches.createNestedObject();
//         switch3["switch"] = "3";
//         switch3["led_status"] = ledState3 ? "ON" : "OFF";

//         String payload;
//         serializeJson(jsonPayload, payload);

//         int httpCode = http.POST(payload);
//         String response = http.getString();
//         Serial.println("HTTP Code: " + String(httpCode));
       
//         if (httpCode == HTTP_CODE_OK) {
//             Serial.println("Response: " + response);
//         } else if (httpCode == HTTP_CODE_TOO_MANY_REQUESTS) {
//             Serial.println("Rate limit exceeded. Retrying later.");
//             delay(60000);
//         } else {
//             Serial.println("Error occurred. HTTP Code: " + String(httpCode));
//         }

//         http.end();
//     } else {
//         Serial.println("WiFi not connected. Check connection.");
//     }
// }




#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "TZ Training Institute";
const char* password = "TZ@TZ@124$";
const char* apiEndpoint = "http://apiabtex.tamilzorous.com/api/multiple_switch_on_off";

#define BUTTON_PIN1 D5
#define BUTTON_PIN2 D6
#define BUTTON_PIN3 D7  
#define LED_PIN1    D3
#define LED_PIN2    D2
#define LED_PIN3    D4

String buttonState1 = ""; 
String buttonState2 = "";
String buttonState3 = "";

bool ledState1 = false;
bool ledState2 = false;
bool ledState3 = false;

void setup() {
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);

    pinMode(BUTTON_PIN1, INPUT_PULLUP);
    pinMode(BUTTON_PIN2, INPUT_PULLUP);
    pinMode(BUTTON_PIN3, INPUT_PULLUP);

    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    String currentButtonState1 = digitalRead(BUTTON_PIN1) == LOW ? "1" : "0";
    String currentButtonState2 = digitalRead(BUTTON_PIN2) == LOW ? "1" : "0";
    String currentButtonState3 = digitalRead(BUTTON_PIN3) == LOW ? "1" : "0";

    if (currentButtonState1 != buttonState1) {
        buttonState1 = currentButtonState1;
        if (buttonState1 == "1") {
            ledState1 = true;
            digitalWrite(LED_PIN1, HIGH);
        } else {
            ledState1 = false;
            digitalWrite(LED_PIN1, LOW);
        }
    }

    if (currentButtonState2 != buttonState2) {
        buttonState2 = currentButtonState2;
        if (buttonState2 == "1") {
            ledState2 = true;
            digitalWrite(LED_PIN2, HIGH);
        } else {
            ledState2 = false;
            digitalWrite(LED_PIN2, LOW);
        }
    }

    if (currentButtonState3 != buttonState3) {
        buttonState3 = currentButtonState3;
        if (buttonState3 == "1") {
            ledState3 = true;
            digitalWrite(LED_PIN3, HIGH);
        } else {
            ledState3 = false;
            digitalWrite(LED_PIN3, LOW);
        }
    }

    sendStatusUpdate();
    delay(1000); 
}

void sendStatusUpdate() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, apiEndpoint);
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<300> jsonPayload;
        JsonArray switches = jsonPayload.createNestedArray("switch");

        JsonObject switch1 = switches.createNestedObject();
        switch1["switch"] = "1";
        switch1["led_status"] = ledState1 ? "ON" : "OFF";

        JsonObject switch2 = switches.createNestedObject();
        switch2["switch"] = "2";
        switch2["led_status"] = ledState2 ? "ON" : "OFF";

        JsonObject switch3 = switches.createNestedObject();
        switch3["switch"] = "3";
        switch3["led_status"] = ledState3 ? "ON" : "OFF";

        String payload;
        serializeJson(jsonPayload, payload);

        int httpCode = http.POST(payload);
        String response = http.getString();
        Serial.println("HTTP Code: " + String(httpCode));

        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Response: " + response);
        } else if (httpCode == HTTP_CODE_TOO_MANY_REQUESTS) {
            Serial.println("Rate limit exceeded. Retrying later.");
            delay(60000);
        } else {
            Serial.println("Error occurred. HTTP Code: " + String(httpCode));
        }

        http.end();
    } else {
        Serial.println("WiFi not connected. Check connection.");
    }
}
