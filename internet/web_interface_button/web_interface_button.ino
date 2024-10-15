#include <WiFi.h>
#include <WebServer.h>
 
#define led1 21
#define led2 22
#define led3 23
 
bool ledState[3] = {false, false, false}; // To track LED states
 
const char* ssid = "FTTH";           // Replace with your Wi-Fi SSID
const char* password = "12345678";   // Replace with your Wi-Fi password
 
WebServer server(80);
 
// HTML for the web page
const char* html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>LED Controller</title>
    <style>
        button {
            width: 100px;
            height: 50px;
            font-size: 16px;
            margin: 5px;
            border: none;
            cursor: pointer;
        }
        .on { background-color: green; color: white; }
        .off { background-color: red; color: white; }
    </style>
</head>
<body>
    <h1>LED Controller</h1>
    <button onclick="location.href='/led1/on'" class="on">LED1 ON</button>
    <button onclick="location.href='/led1/off'" class="off">LED1 OFF</button>
    <button onclick="location.href='/led2/on'" class="on">LED2 ON</button>
    <button onclick="location.href='/led2/off'" class="off">LED2 OFF</button>
    <button onclick="location.href='/led3/on'" class="on">LED3 ON</button>
    <button onclick="location.href='/led3/off'" class="off">LED3 OFF</button>
    <button onclick="location.href='/all/off'" class="off">All OFF</button>
</body>
</html>
)";
 
void setup() {
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
 
    Serial.begin(115200);
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
 
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
 
    server.on("/", []() {
        server.send(200, "text/html", html);
    });
 
    server.on("/led1/on", []() {
        ledState[0] = true;
        digitalWrite(led1, HIGH);
        server.send(200, "text/html", html);
    });
 
    server.on("/led1/off", []() {
        ledState[0] = false;
        digitalWrite(led1, LOW);
        server.send(200, "text/html", html);
    });
 
    server.on("/led2/on", []() {
        ledState[1] = true;
        digitalWrite(led2, HIGH);
        server.send(200, "text/html", html);
    });
 
    server.on("/led2/off", []() {
        ledState[1] = false;
        digitalWrite(led2, LOW);
        server.send(200, "text/html", html);
    });
 
    server.on("/led3/on", []() {
        ledState[2] = true;
        digitalWrite(led3, HIGH);
        server.send(200, "text/html", html);
    });
 
    server.on("/led3/off", []() {
        ledState[2] = false;
        digitalWrite(led3, LOW);
        server.send(200, "text/html", html);
    });
 
    server.on("/all/off", []() {
        for (int i = 0; i < 3; i++) {
            ledState[i] = false;
            digitalWrite(i == 0 ? led1 : (i == 1 ? led2 : led3), LOW);
        }
        server.send(200, "text/html", html);
    });
 
    server.begin();
}
 
void loop() {
    server.handleClient();
}

 