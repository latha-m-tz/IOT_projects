#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Engine Speed Reader");

  // Start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");  
     while (1);
  }

  // Add filter to only receive the CAN bus IDs we care about
  if (useStandardAddressing) {
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }
}

void loop() {
  // Send the request for engine speed data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x01); // 
  
  //Engine speed PID
  CAN.endPacket();
                         
  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x04 ||     // Correct length
         CAN.read() != 0x41 ||     // Correct mode
         CAN.read() != 0x01);      // Correct PID for engine speed

  // Read and parse engine speed
  int rpmRaw_LSB = CAN.read(); // Read the LSB of engine speed value
  int rpmRaw_MSB = CAN.read(); // Read the MSB of engine speed value
  //int rpmRaw = (rpmRaw_MSB << 8) | rpmRaw_LSB; // Combine LSB and MSB to get raw engine speed value
  int rpmRaw =((rpmRaw_MSB*256)+rpmRaw_LSB)/4;
  

  // Calculate engine speed                 
  //float rpm = rpmRaw*0.25;
  float rpm = rpmRaw*0.5;

  // Print engine speed
  Serial.print("Engine Speed (RPM): ");
  Serial.print(rpm); // Print the engine speed value in RPM
  Serial.println(" RPM");

  // Indicate end of processing
 // Serial.println("That's all folks!");

  // A0dd a delay before starting the next iteration
  delay(5000); // 5-second delay
}


/*#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Engine Speed Reader");

  // Start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // Add filter to only receive the CAN bus IDs we care about
  if (useStandardAddressing) 
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }
}

void loop() {
  // Send the request for engine speed data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x03); // Number of additional bytes
  CAN.write(0x22); // Service ID for Read Data by Identifier
  CAN.write(0x01); // Data Identifier (first byte)  
  CAN.write(0x01); // Data Identifier (second byte)
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x04 ||     // Correct length
         CAN.read() != 0x62 ||     // Correct service ID response
         CAN.read() != 0x01 ||     // Correct Data Identifier (first byte)
         CAN.read() != 0x01);      // Correct Data Identifier (second byte)

  // Read and parse engine speed
  int rpmRaw = CAN.read(); // Read the raw engine speed value

  // Calculate engine speed
  float rpm = 0.5 * rpmRaw;

  // Print engine speed
  Serial.print("Engine Speed (RPM): ");
  Serial.print(rpm); // Print the engine speed value in RPM
  Serial.println(" RPM");

  // Add a delay before starting the next iteration
  delay(5000); // 5-second delay
} 
*/