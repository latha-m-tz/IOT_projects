#include <CAN.h>

const bool useStandardAddressing = true;

int i = 0;

// Enumerate the methods
enum Method {
  Parameter_1,
  Parameter_2,
  Parameter_3,
  Parameter_4,
  Parameter_5,
  Parameter_6,
  Parameter_7,
  Parameter_8,
  Parameter_9,
  Parameter_10
};

// Choose the method to call
// Method selectedParameter = Parameter_4; // Change this to choose the desired method

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  if (useStandardAddressing) {
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }
}

void loop() {

  switch (i) {
    case 0:
      readBatteryVoltage();
      break;
    case 1:
      readUnsuccessfulCrankings();
      break;
    case 2:
      readDistanceMIL();
      break;
    case 3:
      readKilometerRunCounter();
      break;
    case 4:
      readO2SensorState();
      break;
    case 5:
      readMAF();
      break;
    case 6:
      readOdometer();
      break;
    case 7:
      readActiveFaults();
      break;
    case 8:
      readIntakeAirTemperature();
      break;
    case 9:
      readEngineStatus();
      break;
  }

  if (i <= 9) {
    i = i + 1;
  } else {
    i = 0;
  }
  Serial.print(" i: ");
  Serial.println(i);
  delay(1000); // Adjust delay as needed
}

void readBatteryVoltage() {
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
         CAN.read() == 0x02 ||         // Correct length
         CAN.read() != 0x41 ||     // Correct mode
         CAN.read() != 0x42);     // Correct PID

  // Read and parse battery voltage
  int voltage_MSB = CAN.read();
  int voltage_LSB = CAN.read();
  float batteryVoltage = ((voltage_MSB * 256) + voltage_LSB) * 0.001; // Convert to volts

  // Print battery voltage
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2); // Print with 2 decimal places
  Serial.println(" V");
}

void readUnsuccessfulCrankings() {
  // Send the rxequest for number of unsuccessful crankings
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x1C); // Number of unsuccessful crankings PID
  CAN.endPacket();  

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x02 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x1C) {     // Correct PID for number of unsuccessful crankings
    // Read and parse the number of unsuccessful crankings
    int unsuccessfulCrankings = CAN.read(); // Number of unsuccessful crankings

    // Print the number of unsuccessful crankings
    Serial.print("Number of Unsuccessful Crankings: ");
    Serial.println(unsuccessfulCrankings);
  }
}

void readDistanceMIL() {
  // Send the request for distance traveled with MIL on data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // Replace  with the appropriate PID for MIL status and distance traveled
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x21) {
    // Read and parse the data for MIL status and distance traveled
    int distanceRaw_LSB = CAN.read(); // Distance traveled with MIL on LSB
    int distanceRaw_MSB = CAN.read(); // Distance traveled with MIL on MSB
    unsigned int distanceRaw = (distanceRaw_MSB << 8) | distanceRaw_LSB;

    // Apply the formula to calculate distance traveled with MIL on
    float distance = distanceRaw * 0.1;

    // Print the distance traveled with MIL on
    Serial.print("Distance Traveled with MIL on: ");
    Serial.print(distance); // Print the distance value
    Serial.println(" miles");
  }
}

void readKilometerRunCounter() {
  // Send the request for Kilometer Run Counter data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // PID for Kilometer Run Counter
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x21) {     // Correct PID for Kilometer Run Counter
    // Read and parse the Kilometer Run Counter value
    int kilometerRunCounter_LSB = CAN.read(); // Least significant byte
    int kilometerRunCounter_MSB = CAN.read(); // Most significant byte
    int kilometerRunCounter = (kilometerRunCounter_MSB << 8) | kilometerRunCounter_LSB;

    // Print Kilometer Run Counter
    Serial.print("Kilometer Run Counter: ");
    Serial.print(kilometerRunCounter);
    Serial.println(" km");
  }
  delay(5000);
}

void readO2SensorState() {
  // Send the request for O2 Sensor State data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x13); // PID for O2 Sensor State
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x13) {     // Correct PID for O2 Sensor State
    // Read and parse the O2 Sensor State value
    int o2SensorState = CAN.read(); // O2 Sensor State value

    // Print O2 Sensor State
    Serial.print("O2 Sensor State: ");
    Serial.println(o2SensorState);
  }

  delay(5000);
}

void readMAF() {
  // Send the request for Mass Air Flow (MAF) data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x10); // MAF PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  if (CAN.read() == 0x04 ||
      CAN.read() == 0x41 ||
      CAN.read() == 0x10) {
    int mafRaw_LSB = CAN.read();
    int mafRaw_MSB = CAN.read();
    int mafRaw = (mafRaw_MSB << 8) | mafRaw_LSB;
    float mafRate = mafRaw * 0.1;
    Serial.print("Mass Air Flow (MAF) Rate (g/s): ");
    Serial.println(mafRate);
    delay(5000);
  }
}

void readOdometer() {
  // Send the request for Starter Relay Status data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x00); // Odometer PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x00) {     // Correct PID for odometer
    // Read and parse the odometer value
    int odometer_msb = CAN.read(); // Most significant byte
    int odometer_lsb = CAN.read(); // Least significant byte

    // Combine the two bytes to form the odometer value
    unsigned int odometer_raw = (odometer_msb << 8) | odometer_lsb;

    // Convert raw odometer value to actual distance traveled in kilometers
    float odometer_km = odometer_raw * 0.1;

    // Print the odometer value
    Serial.print("Odometer: ");
    Serial.print(odometer_km);
    Serial.println(" km");
  }
}

void readActiveFaults() {
  // Send the request for Active Faults data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02);
  CAN.write(0x01);
  CAN.write(0x01);
  CAN.write(0x0A);
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  if (CAN.read() == 0x01 ||
      CAN.read() != 0x41 ||
      CAN.read() != 0x0A) {
    int numFaults = CAN.read();
    Serial.print("Number of Active Faults: ");
    Serial.println(numFaults);
    delay(5000);
  }
}

void readIntakeAirTemperature() {
  // Send the request for Intake Air Temperature data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02);
  CAN.write(0x01);
  CAN.write(0x0F);
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  if (CAN.read() == 0x03 ||
      CAN.read() != 0x41 ||
      CAN.read() != 0x0F) {
    int tempRaw = CAN.read();
    float intakeAirTemperature = tempRaw - 40;
    Serial.print("Intake Air Temperature (°C): ");
    Serial.println(intakeAirTemperature);
    delay(5000);
  }
}

void readEngineStatus() {
  // Send the request for Engine Status data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02);
  CAN.write(0x01);
  CAN.write(0x01);
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  if (CAN.read() == 0x04 ||
      CAN.read() == 0x41 ||
      CAN.read() == 0x01) {
    int keyOnStatus = CAN.read();
    Serial.println(keyOnStatus == 0x01 ? "Engine is ON" : "Engine is OFF");
  }

  delay(5000);
}
