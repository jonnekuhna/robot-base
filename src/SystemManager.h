#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <Arduino.h>
#include "XboxController.h"

class SystemManager {
public:
  void checkFault(XboxController &xbox, USB &Usb) {
    if (!xbox.isConnected()) {
      Serial.println(F("Controller disconnected, pausing..."));
      while (!xbox.isConnected()) {
        Usb.Task();
      }
      Serial.println(F("Controller reconnected!"));

      // Wait for Guide button press
      while (!xbox.getButtonPress(XBOX)) {
        Serial.println(F("Press the Guide button to continue..."));
        Usb.Task();
      }
    }
  }
};

#endif