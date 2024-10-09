#include <Servo.h>

Servo myServo; // Create a servo object

void setup() {
  myServo.attach(9); // Attach the servo on pin 9
}

void loop() {
  myServo.write(90); // Move to 90 degrees
  delay(1000);       // Wait for 1 second
  myServo.write(0);  // Move back to 0 degrees
  delay(1000);       // Wait for 1 second
}
