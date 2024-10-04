#include <SoftwareSerial.h>
#include <CAN.h>


SoftwareSerial BTSerial(5, 6); // TX/RX pins for Bluetooth module
const bool useStandardAddressing = true;

void setup() {  
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  Serial.println("CAN OBD-II Odometer and Battery Voltage Reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  if (useStandardAddressing) {
    CAN.filter(0x7E8);
  } else {
    CAN.filterExtended(0x18DAF110);
  }
}
 
void loop() {
  if (BTSerial.available()) { 
    String command = BTSerial.readStringUntil('\n'); // Read until newline character
    Serial.println(command);  
    //020902
    //int a ;
    
    if (command.indexOf("22F190") != -1)  {
      requestBatteryVoltage();
    } else if (command.indexOf("220136") != -1)  {      
      requestOdometer(); 
    } else {
      Serial.println("Invalid command");
      BTSerial.println("Invalid command");
    }
  }
}

void requestOdometer() {
  Serial.println("Received '220136', requesting odometer...");
  // send the request for the odometer data   
  if (useStandardAddressing) {
    CAN.beginPacket(0x7DF, 8);
  } else {
    CAN.beginExtendedPacket(0x18DB33F1, 8);
  }
  CAN.write(0x02); // Number of additionalp bytes
  CAN.write(0x01); // Request current data
  CAN.write(0xA6); // Odometer PID
  CAN.endPacket();

  // wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() == 0x02 ||       // Correct length
         CAN.read() != 0x41 ||       // Correct mode
         CAN.read() != 0xA6);        // Correct PID for odometer

  // Read and parse the odometer value
  int odometer_msb = CAN.read(); // Most significant byte
  int odometer_lsb = CAN.read(); // Least significant byte

  // Combine the two bytes to form the odometer value
  unsigned int odometer_raw = (odometer_msb << 8) | odometer_lsb;

  // Convert raw odometer value to actual distance traveled in kilometers
  float odometer_km = odometer_raw * 0.1;

  // Print the odometer valueven
  Serial.print("Odometer: ");
  Serial.print(odometer_km);
  Serial.println(" km");
  
  // Send the odometer value over Bluetooth
  BTSerial.print("Odometer: ");
  BTSerial.print(odometer_km);
  BTSerial.println(" km");
}

void requestBatteryVoltage() {
  Serial.println("Received '22F190', requesting battery voltage...");
  // Send the request for battery voltage data
  if (useStandardAddressing) {  
    CAN.beginPacket(0x7df, 8);
  } else { 
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x42); // Battery voltage PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() == 0x02  ||         // Correct length
         CAN.read() != 0x41  ||     // Correct mode
         CAN.read() != 0x42);     // Correct PID

  // Read and parse battery voltage
  int voltage_MSB = CAN.read();
  int voltage_LSB = CAN.read();
  float batteryVoltage = ((voltage_MSB * 256) + voltage_LSB ) * 0.001; //Convert to volts
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2); // Print with 2 decimal places
  Serial.println(" V");

  // Send the battery voltage over Bluetooth
  BTSerial.print("Battery Voltage: ");
  BTSerial.print(batteryVoltage, 2);
  BTSerial.println(" V");
}

