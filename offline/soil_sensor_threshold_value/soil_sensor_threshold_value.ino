const int sensorPin = A0;    // Pin where the sensor is connected
const int ledPin = 9;        // Pin where the LED is connected
int sensorValue = 0;         // Variable to store the sensor reading
const int threshold = 300;   // Define your threshold value

void setup() {
  Serial.begin(9600);        // Start serial communication at 9600 bps
  pinMode(ledPin, OUTPUT);   // Set the LED pin as an output
}

void loop() {
  sensorValue = analogRead(sensorPin);  // Read the sensor value
  Serial.print("Soil Moisture Level: ");
  Serial.println(sensorValue);           // Print the value to the Serial Monitor

  if (sensorValue > threshold) {         // Check if the sensor value is above the threshold
    digitalWrite(ledPin, HIGH);          // Turn the LED on
    delay(500);                          // Keep the LED on for 500 milliseconds
    digitalWrite(ledPin, LOW);           // Turn the LED off
    delay(500);                          // Keep the LED off for 500 milliseconds
  } else {
    digitalWrite(ledPin, LOW);           // Ensure the LED is off if below threshold
  }

  delay(1000);  // Wait for a second before the next reading
}
