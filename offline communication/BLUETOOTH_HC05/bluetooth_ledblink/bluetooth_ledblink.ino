char command; // Variable to store received data

void setup() {
  pinMode(13, OUTPUT); // Set pin 13 as an output
  Serial.begin(9600); // Initialize serial communication at 9600 bps
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read(); // Read the incoming data

    // Check the received command and perform actions
    if (command == '1') {
      digitalWrite(13, HIGH); // Turn on the LED
      Serial.println("LED ON");
    } else if (command == '0') {
      digitalWrite(13, LOW); // Turn off the LED
      Serial.println("LED OFF");
    }
  }
}
