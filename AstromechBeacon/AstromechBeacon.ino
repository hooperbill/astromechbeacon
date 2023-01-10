/*
  Astromech Beacon

  This example creates a BLE peripheral which advertises itself as AstromechBeacon.
  Based on code from https://github.com/amandagrice/astromech

*/

#include <ArduinoBLE.h>  //http://librarymanager/All#ArduinoBLE_IoT

uint8_t location = 0x01;
uint8_t msd_payload[8];

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1) {
      delay(100);
    }
  } else {
    Serial.println("BLE initialized (peripheral mode)");
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("AstromechBeacon");

  // start advertising
  startAdvertising(location);

  Serial.println("BLE advertising as 'AstromechBeacon'");
}

void loop() {
  delay(150000);
  location = (location % 7) + 1;
  stopAdvertising();
  startAdvertising(location);
}

/**
  Stop any advertising. Must stop advertising to change any data
*/
void stopAdvertising() {
  Serial.println("Stop advertisement");
  BLE.stopAdvertise();
}

/**
 Start advertising with new manufacturer data
 */
void startAdvertising(uint8_t location) {
  Serial.println("Starting to advertise ");
  Serial.println(location);

  uint16_t msd_cid = 0x0183;
  memset(msd_payload, 0, sizeof(msd_payload));
  memcpy(msd_payload, (uint8_t*)&msd_cid, sizeof(msd_cid));

  msd_payload[2] = 0x0A;
  msd_payload[3] = 0x04;
  msd_payload[4] = location;
  msd_payload[5] = 0x02;
  msd_payload[6] = 0xA6;
  msd_payload[7] = 0x01;
  BLE.setManufacturerData(msd_payload, sizeof(msd_payload));
  BLE.setConnectable(false);
  BLE.setConnectionInterval(250, 250);
  BLE.setSupervisionTimeout(30);

  BLE.advertise();
}
