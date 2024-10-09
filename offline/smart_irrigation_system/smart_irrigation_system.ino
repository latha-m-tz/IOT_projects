const int moisturePin = A0;  // Pin for soil moisture sensor
const int tempPin = 8;       // Pin for temperature sensor
int moistureValue = 0;        // Variable to store the soil moisture reading
float temperature = 0;         // Variable to store the temperature

void setup() {
  Serial.begin(9600);          // Start serial communication at 9600 bps
}

void loop() {
  // Read soil moisture
  moistureValue = analogRead(moisturePin);
  
  // Read temperature (for LM35, convert the analog value to Celsius)
  int tempReading = analogRead(tempPin);
  temperature = (tempReading * 5.0 * 100.0) / 1024.0; // Convert to Celsius

  Serial.print("Soil Moisture Level: ");
  Serial.println(moistureValue);    // Print soil moisture reading
  Serial.print("Temperature: ");
  Serial.println(temperature);       // Print temperature reading

  // Check conditions
  if (moistureValue > 1000 && temperature > 30) {
    Serial.println("Water the plant.");
  } else {
    Serial.println("Plant is wet.");
  }

  delay(1000);  // Wait for a second before next reading
}
