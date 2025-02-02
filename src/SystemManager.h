#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <Arduino.h>
#include "XboxController.h"  // Include the Xbox controller class

class SystemManager {
public:
    // Constructor (removed resetPin)
    SystemManager(int lfEnable, int rfEnable, int lrEnable, int rrEnable, XboxController* xboxController)
        : leftFrontEnablePin(lfEnable), rightFrontEnablePin(rfEnable),
          leftRearEnablePin(lrEnable), rightRearEnablePin(rrEnable),
          xbox(xboxController) {}

    // Initialize system (enable motors in 2-second intervals)
    void initSystem() {
        Serial.println(F("Starting system initialization..."));

        // Set enable pins as outputs
        pinMode(leftFrontEnablePin, OUTPUT);
        pinMode(rightFrontEnablePin, OUTPUT);
        pinMode(leftRearEnablePin, OUTPUT);
        pinMode(rightRearEnablePin, OUTPUT);

        // Start in fault condition until initialization is complete
        inFault = true;

        // Enable motors sequentially
        enableMotor(leftFrontEnablePin, "Left Front");
        enableMotor(rightFrontEnablePin, "Right Front");
        enableMotor(leftRearEnablePin, "Left Rear");
        enableMotor(rightRearEnablePin, "Right Rear");

        Serial.println(F("Initialization complete. Motors enabled."));
        inFault = false;  // System is now operational
    }

    // Check for faults
    void checkForFaults() {
        // Check if Xbox controller is disconnected
        if (!xbox->isConnected()) {
            Serial.println(F("FAULT DETECTED: Xbox controller disconnected! Disabling motors."));
            disableMotors();
            inFault = true;
            return;
        }

        // other fault detection conditions if needed
    }

    // Disable all motors if a fault occurs
    void disableMotors() {
        Serial.println(F("Disabling all motors..."));
        digitalWrite(leftFrontEnablePin, LOW);
        digitalWrite(rightFrontEnablePin, LOW);
        digitalWrite(leftRearEnablePin, LOW);
        digitalWrite(rightRearEnablePin, LOW);
    }

    // Check if the Xbox Guide Button is pressed for reset
    void checkForReset() {
        if (xbox->isConnected() && xbox->getButtonPress(XBOX_VID)) {
            Serial.println(F("Xbox Guide Button pressed. Restarting system..."));
            resetSystem();
        }
    }

    // Reset system (Re-run initialization)
    void resetSystem() {
        disableMotors(); // Turn everything off first
        initSystem();    // Restart initialization
    }

    // Getter for fault status
    bool isFault() {
        return inFault;
    }

private:
    int leftFrontEnablePin;
    int rightFrontEnablePin;
    int leftRearEnablePin;
    int rightRearEnablePin;
    XboxController* xbox;
    bool inFault = true;

    // Helper function to enable a motor with delay
    void enableMotor(int pin, const char* motorName) {
        digitalWrite(pin, HIGH);
        Serial.print(motorName);
        Serial.println(F(" Motor Enabled"));
        delay(2000);
    }
};

#endif // SYSTEMMANAGER_H
