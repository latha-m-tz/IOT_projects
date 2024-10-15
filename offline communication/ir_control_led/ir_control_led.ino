#include <IRremote.h> 
int RECV_PIN = 11;          // Pin for the IR receiver
int LED_PIN = 13;           // Pin for the LED (you can change it to any other pin)
unsigned long targetValue = 0x25AE7EE3; // The specific hex value 
IRrecv irrecv(RECV_PIN);    // Create an instance of IRrecv
decode_results results;     // Create an instance of decode_results 
void setup() {
  Serial.begin(9600);       // Start the serial communication
  irrecv.enableIRIn();      // Start the receiver
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as output
  digitalWrite(LED_PIN, LOW); // Initially turn off the LED
} 
void loop() {
  if (irrecv.decode(&results)) {  // If a signal is received
    Serial.println(results.value, HEX);  // Print the received value in HEX 
    if (results.value == targetValue) {  // Check if the value matches 0x25AE7EE3
      digitalWrite(LED_PIN, HIGH);       // Turn on the LED
    } else {
      digitalWrite(LED_PIN, LOW);        // Turn off the LED
    } 
    irrecv.resume();  // Receive the next value
  }
  delay(1000);  // Small delay to prevent overwhelming the serial monitor
}


 