#include <SoftwareSerial.h>
#include <CAN.h>

SoftwareSerial mySerial(5, 6);
bool requestVIN = false;


void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  Serial.println("CAN OBD-II VIN reader");
  mySerial.print("CAN OBD II VIN reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    mySerial.println("Starting CAN failed!");
    while (1);
  }
    CAN.filter(0x7e8);
  
}

void loop() {
  if (mySerial.available()) {
    String command = mySerial.readStringUntil('\n');
    command.trim();
    Serial.print("Data received from BLE: ");
    Serial.println(command);
    delay(1000);

    if (command == "22f190") {

      //Serial.print(i);
      readVIN();
      //delay(1000);
    } else {
      Serial.println("invalid  command.");
      mySerial.println("invalid  command.");

      delay(1000);
    }
  }
}

void readVIN() {
  Serial.println("Sending VIN request...");
  CAN.beginPacket(0x7df, 8);
  CAN.write(0x02); 
  CAN.write(0x09); 
  CAN.write(0x02); 
  CAN.endPacket();


  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x10 || 
         CAN.read() != 0x14 ||
         CAN.read() != 0x49 || 
         CAN.read() != 0x02 || 
         CAN.read() != 0x01) {
  }

  
  while (CAN.available()) {
    char c = (char)CAN.read();
    Serial.write(c);
    mySerial.write(c);
  }

  for (int i = 0; i < 2; i++) {
    CAN.beginPacket(0x7e0, 8);
    CAN.write(0x30);
    CAN.endPacket(); 

    while (CAN.parsePacket() == 0 ||
           CAN.read() != (0x21 + i)) {
    }

    while (CAN.available()) {
      char c = (char)CAN.read();
      Serial.write(c); 
      mySerial.write(c);
    }
  }

  Serial.println("That's all folks!");
}








/*#include <SoftwareSerial.h>
#include <CAN.h>

SoftwareSerial mySerial(5, 6);
const bool useStandardAddressing = true;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  
  Serial.println("CAN OBD-II Reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  CAN.filter(0x7e8);
}

void loop() {
  if (mySerial.available()) {
    char i = mySerial.read();
    Serial.print("Data received from BLE: ");
    Serial.println(i);

    if (i == '1') {
      readVIN();
    } else if (i == '2') {
      readBatteryVoltage();
    } else {
      Serial.println("Invalid command.");
    }
  }
}

void readVIN() {
  Serial.println("Sending VIN request...");
  CAN.beginPacket(0x7df, 8);
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x09); // Request vehicle information
  CAN.write(0x02); // Vehicle Identification Number (VIN)
  CAN.endPacket();

  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x10 || 
         CAN.read() != 0x14 ||
         CAN.read() != 0x49 || 
         CAN.read() != 0x02 || 
         CAN.read() != 0x01) {
  }

  while (CAN.available()) {
    char c = (char)CAN.read();
    Serial.write(c);
    mySerial.write(c);
  }

  for (int i = 0; i < 2; i++) {
    CAN.beginPacket(0x7e0, 8);
    CAN.write(0x30);
    CAN.endPacket(); 

    while (CAN.parsePacket() == 0 ||
           CAN.read() != (0x21 + i)) {
    }

    while (CAN.available()) {
      char c = (char)CAN.read();
      Serial.write(c); 
      mySerial.write(c);
    }
  }

  Serial.println("VIN request complete.");
}

void readBatteryVoltage() {
  // Send the request for battery voltage data
    CAN.beginPacket(0x7df, 8);

  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x42); // Battery voltage PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() == 0x02  ||         // Correct length
         CAN.read() != 0x41  ||     // Correct mode0
         CAN.read() != 0x42);     // Correct PID

  // Read and parse battery voltage
  int voltage_MSB = CAN.read();
  int voltage_LSB = CAN.read();
  float batteryVoltage = ((voltage_MSB * 256) + voltage_LSB ) * 0.001; // Convert to volts

  // Print battery voltage
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2); // Print with 2 decimal places
  Serial.println(" V");
}*/