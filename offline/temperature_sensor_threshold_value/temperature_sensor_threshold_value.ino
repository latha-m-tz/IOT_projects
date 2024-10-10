#include <dht.h>

dht DHT; // Create an instance of the DHT class

#define DHT11_PIN 7    // Define the pin where the DHT11 sensor is connected
#define LED_PIN 9      // Define the pin where the LED is connected
#define TEMP_THRESHOLD 30 // Define the temperature threshold (in °C)

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 bps
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
}

void loop() {
  int chk = DHT.read11(DHT11_PIN); // Read data from the DHT11 sensor

  // Check if the reading was successful
  if (chk == DHTLIB_OK) {
    Serial.print("Temperature = ");
    Serial.print(DHT.temperature); // Print the temperature
    Serial.println(" °C"); // Append degree Celsius

    Serial.print("Humidity = ");
    Serial.print(DHT.humidity); // Print the humidity
    Serial.println(" %"); // Append percentage

    // Check if temperature exceeds the threshold
    if (DHT.temperature > TEMP_THRESHOLD) {
      // Blink the LED
      digitalWrite(LED_PIN, HIGH); // Turn the LED on
      delay(500);                  // Keep the LED on for 500 milliseconds
      digitalWrite(LED_PIN, LOW);  // Turn the LED off
      delay(500);                  // Keep the LED off for 500 milliseconds
    } else {
      digitalWrite(LED_PIN, LOW); // Ensure the LED is off if below threshold
    }
  } else {
    Serial.println("Error reading from DHT11 sensor!"); // Print error message
  }

  delay(1000); // Wait for a second before the next reading
}
