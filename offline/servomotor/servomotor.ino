
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos= 0;    // variable to read the value from the analog pin

void setup() {
  myservo.attach(9);
  myservo.write(95) ; // attaches the servo on pin 9 to the servo object
}

void loop() {        // reads the value of the potentiometer (value between 0 and 1023)
}
