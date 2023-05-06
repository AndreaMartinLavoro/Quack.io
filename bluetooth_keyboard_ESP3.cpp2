// Library URL: https://github.com/T-vK/ESP32-BLE-Keyboard
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();
}

void loop() {
  string msg = "";
  if(bleKeyboard.isConnected()) {
    bleKeyboard.print(msg);
    bleKeyboard.write(KEY_RETURN);
  }
}
