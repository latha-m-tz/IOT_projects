const int BUTTON1 = 2; // Initialize Pin 2 with Button 1
const int BUTTON2 = 4; // Initialize Pin 4 with Button 2
const int BUTTON3 = 7; // Initialize Pin 7 with Button 3
const int LED1 = 8;    // Initialize Pin 8 for LED 1
const int LED2 = 12;   // Initialize Pin 12 for LED 2
const int LED3 = 13;   // Initialize Pin 13 for LED 3

int BUTTONstate1 = 0;  // To read the state of Button 1
int BUTTONstate2 = 0;  // To read the state of Button 2
int BUTTONstate3 = 0;  // To read the state of Button 3

void setup() {
  pinMode(BUTTON1, INPUT); // Define Button 1 as an input pin
  pinMode(BUTTON2, INPUT); // Define Button 2 as an input pin
  pinMode(BUTTON3, INPUT); // Define Button 3 as an input pin
  pinMode(LED1, OUTPUT);   // Define LED 1 as an output pin
  pinMode(LED2, OUTPUT);   // Define LED 2 as an output pin
  pinMode(LED3, OUTPUT);   // Define LED 3 as an output pin
}

void loop() {
  BUTTONstate1 = digitalRead(BUTTON1); // Read Button 1 state
  if (BUTTONstate1 == LOW) {
    digitalWrite(LED1, HIGH); // Turn LED 1 ON
  } else {
    digitalWrite(LED1, LOW); // Turn LED 1 OFF
  }

  BUTTONstate2 = digitalRead(BUTTON2); // Read Button 2 state
  if (BUTTONstate2 == HIGH) {
    digitalWrite(LED2, LOW); // Turn LED 2 OFF
  } else {
    digitalWrite(LED2, HIGH); // Turn LED 2 ON
  }

  BUTTONstate3 = digitalRead(BUTTON3); // Read Button 3 state
  if (BUTTONstate3 == HIGH) {
    digitalWrite(LED3, LOW); // Turn LED 3 OFF
  } else {
    digitalWrite(LED3, HIGH); // Turn LED 3 ON
  }
}
  