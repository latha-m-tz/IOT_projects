#include <Servo.h>

Servo myServo; // Create a servo object

int IRSensor = 8; // Connect IR sensor module to Arduino pin D8
int LED = 13;      // Connect LED to Arduino pin 13

void setup() {
  Serial.begin(115200); // Init Serial at 115200 Baud Rate.
  Serial.println("Serial Working"); // Test to check if serial is working
  pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
  pinMode(LED, OUTPUT); // LED Pin Output
  myServo.attach(9); // Attach the servo on pin 9
}

void loop() {
  int sensorStatus = digitalRead(IRSensor); // Read the status of the IR sensor

  if (sensorStatus == HIGH) { // If motion is detected
    digitalWrite(LED, LOW); // Turn off the onboard LED
    Serial.println("Motion Detected!"); // Print Motion Detected! on the serial monitor

    myServo.write(90); // Move the servo to 90 degrees
    delay(2000);       // Wait for 1 second
    myServo.write(0);  // Move the servo back to 0 degrees
    delay(2000);       // Wait for 1 second
  } else { // If no motion is detected
    digitalWrite(LED, HIGH); // Turn on the onboard LED
    Serial.println("Motion Ended!"); // Print Motion Ended! on the serial monitor

    // Blink the LED when motion ends
      // digitalWrite(LED, LOW); // Turn LED off
      // delay(200);             // Wait for 200 ms
      digitalWrite(LED, HIGH); // Turn LED on
      delay(2000);             // Wait for 200 ms
    }
  }
}
