#include <Servo.h>

#define TRIG_PIN 10   // Pin connected to the TRIG pin of the ultrasonic sensor
#define ECHO_PIN 11  // Pin connected to the ECHO pin of the ultrasonic sensor
#define SERVO_PIN 9  // Pin connected to the servo motor

Servo myServo;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(TRIG_PIN, OUTPUT); // Set TRIG pin as OUTPUT
  pinMode(ECHO_PIN, INPUT);  // Set ECHO pin as INPUT
  myServo.attach(SERVO_PIN); // Attach the servo to the specified pin
  myServo.write(0);           // Initial position of the servo (closed position)
}

void loop() {
  // Clear the TRIG pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Set the TRIG pin high for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the ECHO pin
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance in cm
  long distance = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If the distance is below 20 cm, open the door (rotate servo)
  if (distance < 20) {
    myServo.write(90); // Rotate servo to 90 degrees (open position)
    Serial.println("Door is opened.");
  } else {
    myServo.write(0);  // Rotate servo to 0 degrees (closed position)
    Serial.println("Door is closed.");
  }

  delay(500); // Wait half a second before the next measurement
}
