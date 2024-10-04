#include <ESP8266WiFi.h>

String i = "";
const char* ssid = "TamilZorous";
const char* password = "TZ@12477";

WiFiServer server(80);

// Pin
int output_pin = 2;
int led_pin = 15;  // LED control pin

void setup() {
  // Start Serial
  Serial.begin(115200);
  delay(10);

  // Prepare GPIO5
  pinMode(output_pin, OUTPUT);
  digitalWrite(output_pin, 0);

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Initialization complete.");
  Serial.print("Server IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.print("Server started, URL: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  client.flush();

  if (req.indexOf("/on") != -1) {
    i = "1";
  } else if (req.indexOf("/off") != -1) {
    i = "0";
  } else if (req.indexOf("/ledon") != -1) {
    i = "11";
  } else if (req.indexOf("/ledoff") != -1) {
    i = "22";
  } else {
    // Default response for the root URL
    i = "";
  }

  if (i == "1") {
    digitalWrite(output_pin, 1);
    Serial.println("Bulb ON");
  } else if (i == "0") {
    digitalWrite(output_pin, 0);
    Serial.println("Bulb OFF");
  } else if (i == "11") {
    digitalWrite(led_pin, 1);
    Serial.println("LED ON");
  } else if (i == "22") {
    digitalWrite(led_pin, 0);
    Serial.println("LED OFF");
  }

  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE html>";
  s += "<html lang=\"en\">";
  s += "<head>";
  s += "<meta charset=\"UTF-8\">";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<title>Relay and LED Control</title>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "<style>";
  s += "body { padding-top: 50px; }";
  s += ".btn-container { text-align: center; }";
  s += ".btn-container .btn { margin: 10px; width: 150px; font-size: 18px; color: white; }";
  s += ".btn-container .btn-on { background-color: #5cb85c; border-color: #4cae4c; }";
  s += ".btn-container .btn-off { background-color: #d9534f; border-color: #d43f3a; }";
  s += ".btn-container .btn-led-on { background-color: #5cb85c; border-color: #4cae4c; }";
  s += ".btn-container .btn-led-off { background-color: #d9534f; border-color: #d43f3a; }";
  s += "</style>";
  s += "</head>";
  s += "<body>";
  s += "<div class=\"container\">";
  s += "<h1 class=\"text-center\">Bulb Control Using Relay and LED Control</h1>";
  s += "<div class=\"btn-container\">";
  s += "<button class=\"btn btn-lg btn-danger btn-off\" onclick=\"off()\">Bulb ON</button>";
  s += "<button class=\"btn btn-lg btn-success btn-on\" onclick=\"on()\">Bulb OFF</button>";
  s += "<br>";
  s += "<button class=\"btn btn-lg btn-success btn-led-on\" onclick=\"ledon()\">LED ON</button>";
  s += "<button class=\"btn btn-lg btn-danger btn-led-off\" onclick=\"ledoff()\">LED OFF</button>";
  s += "</div>";
  s += "</div>";
  s += "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>";
  s += "<script>";
  s += "function on() {$.get(\"/on\");}";
  s += "function off() {$.get(\"/off\");}";
  s += "function ledon() {$.get(\"/ledon\");}";
  s += "function ledoff() {$.get(\"/ledoff\");}";
  s += "</script>";
  s += "</body>";
  s += "</html>";

  client.print(s);
  delay(1);
}