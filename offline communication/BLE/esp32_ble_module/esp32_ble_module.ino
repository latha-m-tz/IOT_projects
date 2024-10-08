#include <BLEDevice.h>

#include <BLEUtils.h>

#include <BLEServer.h>




#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"




BLEServer* pServer = nullptr;

BLECharacteristic* pCharacteristic = nullptr;

bool deviceConnected = false;




class MyServerCallbacks: public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) {

      deviceConnected = true;

    };




    void onDisconnect(BLEServer* pServer) {

      deviceConnected = false;

    }

};




class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic* pCharacteristic) {

      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {

        Serial.print("Received Value: ");

        for (int i = 0; i < value.length(); i++) {

          Serial.print(value[i]);

        }

        Serial.println();

      }

    }

};




void setup() {

  Serial.begin(115200);

  Serial.println("Starting BLE work!");




  BLEDevice::init("ESP32_BLE_Server");

  pServer = BLEDevice::createServer();

  pServer->setCallbacks(new MyServerCallbacks());




  BLEService *pService = pServer->createService(SERVICE_UUID);




  pCharacteristic = pService->createCharacteristic(

                      CHARACTERISTIC_UUID,

                      BLECharacteristic::PROPERTY_READ |

                      BLECharacteristic::PROPERTY_WRITE

                    );




  pCharacteristic->setCallbacks(new MyCallbacks());




  pCharacteristic->setValue("Hello World");

  pService->start();




  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);

  pAdvertising->setScanResponse(true);

  pAdvertising->setMinPreferred(0x06);  
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it in your phone!");

}




void loop() {


  delay(1000);

}



