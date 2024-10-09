const int sensorPin = A0;  // Pin where the sensor is connected
int sensorValue = 0;       // Variable to store the sensor reading

void setup() {
  Serial.begin(9600);      // Start serial communication at 9600 bps
}

void loop() {
  sensorValue = analogRead(sensorPin);  // Read the sensor value
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue);           // Print the value to the Serial Monitor

  delay(1000);  // Wait for a second before the next reading
}
