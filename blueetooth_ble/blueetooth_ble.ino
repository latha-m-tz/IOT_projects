#include <BluetoothSerial.h>
#include <CAN.h>

// Create Bluetooth serial object
BluetoothSerial BTSerial; 
const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  BTSerial.begin("ESP32_CAN"); // Initialize Bluetooth with the name "ESP32_CAN"
  
  Serial.println("CAN OBD-II VIN and Battery Voltage Reader");

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
    
    if (command.indexOf("220106") != -1)  {
      requestBatteryVoltage();
    } else if (command.indexOf("22F190") != -1)  {      
      requestVIN(); 
    } else {
      Serial.println("Invalid command");
      BTSerial.println("Invalid command");
    }
  }
}

void requestVIN() {
  Serial.println("Received '22F190', requesting VIN...");
  // send the request for VIN data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);  
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x09); // Request vehicle information
  CAN.write(0x02); // Vehicle Identification Number (VIN)
  CAN.endPacket();

  // wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x10 || CAN.read() != 0x14 || // correct length
         CAN.read() != 0x49 ||                       // correct mode
         CAN.read() != 0x02 ||                       // correct PID
         CAN.read() != 0x01);

  String vin = "";
  // Print out VIN characters to both Serial monitor and Bluetooth terminal
  while (CAN.available()) {
    char vinChar = (char)CAN.read();
    Serial.write(vinChar);
    vin += vinChar;
  }

  // read in remaining chunks
  for (int i = 0; i < 2; i++) { // Changed loop to read remaining 2 characters
    // send the request for the next chunk
    if (useStandardAddressing) {
      CAN.beginPacket(0x7e0, 8);
    } else {
      CAN.beginExtendedPacket(0x18db33f1, 8);
    }
    CAN.write(0x30);
    CAN.endPacket();

    // wait for response
    while (CAN.parsePacket() == 0 ||
           CAN.read() != (0x21 + i)); // correct sequence number
 
    // Print VIN characters
    while (CAN.available()) {
      char vinChar = (char)CAN.read();
      Serial.write(vinChar); 
      vin += vinChar;
    }
  }
  
  Serial.println();
  BTSerial.println(vin);
}

void requestBatteryVoltage() {
  Serial.println("Received '220106', requesting battery voltage...");
  // Send the request for battery voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7DF, 8);
  } else { 
    CAN.beginExtendedPacket(0x18DB33F1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x42); // Battery voltage PID
  CAN.endPacket();                          

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x04  ||         // Correct length
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
