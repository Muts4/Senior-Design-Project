#include <TinyPICO.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID "INSERT GENERATED UUID HERE"
#define CHARACTERISTIC_UUID "INSERT GENERATED UUID HERE"
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
BLEDevice::init("MyESP32");
BLEServer *pServer = BLEDevice::createServer();
BLEService *pService = pServer->createService(SERVICE_UUID);
BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                     CHARACTERISTIC_UUID,
                                     BLECharacteristic::PROPERTY_READ |
                                     BLECharacteristic::PROPERTY_WRITE);
pCharacteristic->setValue("Hello World says Matt");
BLEAdvertising *pAdvertising = pServer->getAdvertising();
pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:

}
