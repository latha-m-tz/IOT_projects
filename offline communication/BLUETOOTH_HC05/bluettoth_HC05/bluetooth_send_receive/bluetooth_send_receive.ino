#include <SoftwareSerial.h>

SoftwareSerial BTSerial(5,6); // TX/RX

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);  // Default baud rate of the Bluetooth module

  Serial.println("Arduino is ready");

}

void loop() {
  if (BTSerial.available()) { 
    char c = BTSerial.read();
    Serial.write(c); // Echo character to serial monitor
  }
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c); // Send character to Bluetooth module
  }
}  
                                  