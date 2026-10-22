// ===================================================================
//  BluetoothKeys.h  —  makes the ESP32 a Bluetooth keyboard
//  Uses only the BLE library that is built into the ESP32.
//  In setup():  btBegin("My Remote");
//  Anywhere:    if (btConnected()) btPress(KEY_RIGHT);
//  You do NOT need to understand every line of this file.
// ===================================================================
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include <BLE2902.h>
 
// Key codes from the USB "HID" standard (every keyboard uses them)
const uint8_t KEY_A_LETTER = 0x04;   // 'a'. 'b' = 0x05, 'c' = 0x06 ... 'z' = 0x1D
const uint8_t KEY_ENTER = 0x28;
const uint8_t KEY_ESCAPE = 0x29;
const uint8_t KEY_F5_KEY = 0x3E;
const uint8_t KEY_RIGHT = 0x4F;
const uint8_t KEY_LEFT = 0x50;
const uint8_t KEY_DOWN = 0x51;
const uint8_t KEY_UP = 0x52;
const uint8_t MOD_SHIFT = 0x02;      // hold Shift while pressing
 
// The "report map": tells the computer "I am a normal keyboard"
static const uint8_t btReportMap[] = {
  0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01,   // keyboard, report ID 1
  0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00,   // 8 modifier keys
  0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02,
  0x95, 0x01, 0x75, 0x08, 0x81, 0x01,               // 1 reserved byte
  0x95, 0x05, 0x75, 0x01, 0x05, 0x08, 0x19, 0x01,   // 5 LEDs (Caps Lock ...)
  0x29, 0x05, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x01,
  0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x25, 0x65,   // up to 6 keys at once
  0x05, 0x07, 0x19, 0x00, 0x29, 0x65, 0x81, 0x00,
  0xC0
};
 
BLEHIDDevice* btHid;
BLECharacteristic* btInput;
bool btIsConnected = false;
 
class BtCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) {
    btIsConnected = true;
    BLE2902* d = (BLE2902*)btInput->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    if (d) d->setNotifications(true);
  }
  void onDisconnect(BLEServer* server) {
    btIsConnected = false;
    BLE2902* d = (BLE2902*)btInput->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    if (d) d->setNotifications(false);
    server->getAdvertising()->start();   // let the computer find us again
  }
};
 
// Start Bluetooth with the name the computer will see
void btBegin(const char* name) {
  BLEDevice::init(name);
  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new BtCallbacks());
 
  btHid = new BLEHIDDevice(server);
  btInput = btHid->inputReport(1);
  btHid->outputReport(1);
  btHid->manufacturer()->setValue("Circuit Decoder");
  btHid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  btHid->hidInfo(0x00, 0x01);
  btHid->reportMap((uint8_t*)btReportMap, sizeof(btReportMap));
  btHid->startServices();
  btHid->setBatteryLevel(100);
 
  BLESecurity* security = new BLESecurity();
  security->setAuthenticationMode(ESP_LE_AUTH_BOND);   // remember the pairing
 
  BLEAdvertising* adv = server->getAdvertising();
  adv->setAppearance(HID_KEYBOARD);
  adv->addServiceUUID(btHid->hidService()->getUUID());
  adv->start();
}
 
bool btConnected() {
  return btIsConnected;
}
 
// Press and release one key (with an optional modifier like MOD_SHIFT)
void btPress(uint8_t keyCode, uint8_t modifier = 0) {
  if (!btIsConnected) return;
  uint8_t report[8] = {modifier, 0, keyCode, 0, 0, 0, 0, 0};
  btInput->setValue(report, sizeof(report));
  btInput->notify();                    // key down
  delay(10);
  uint8_t release[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  btInput->setValue(release, sizeof(release));
  btInput->notify();                    // key up
  delay(10);
}
 
// Type a digit '0' to '9'
void btDigit(char digit) {
  if (digit == '0') btPress(0x27);
  else if (digit >= '1' && digit <= '9') btPress(0x1E + (digit - '1'));
}
