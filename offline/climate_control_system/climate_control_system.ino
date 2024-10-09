#include <DHT.h>
#include <Servo.h>

#define DHTPIN 7       // Pin where the DHT11 is connected
#define DHTTYPE DHT11  // DHT 11
#define SERVO_PIN 9    // Pin where the servo is connected

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  dht.begin();        // Initialize the DHT sensor
  myServo.attach(SERVO_PIN); // Attach the servo to the specified pin
  myServo.write(0);   // Initial position of the servo (closed position)
}

void loop() {
  delay(2000); // Wait a few seconds between measurements

  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Check if reading failed and exit early
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the temperature value to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  // If the temperature is above 30 degrees, open the window (rotate servo)
  if (temperature > 30) {
    myServo.write(90); // Rotate servo to 90 degrees (open position)
    Serial.println("Too hot! Window is opened.");
  } else {
    myServo.write(0);  // Rotate servo to 0 degrees (closed position)
    Serial.println("Temperature is normal. Window is closed.");
  }
}
