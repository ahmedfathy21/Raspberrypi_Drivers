#include <Wire.h>

#define I2C_ADDRESS 0x08  // Slave address (match this in Pi code)
#define COMMAND_LEN 4      // Length of command (e.g., "OFF" + null terminator)

// Default command (initialize to "OFF")
volatile char command[COMMAND_LEN] = "OFF";

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_ADDRESS);    // Initialize I2C slave mode
  Wire.onRequest(sendCommand); // Register callback for Pi requests
}

void loop() {
  // Update the command (e.g., via a button, HTTP request, etc.)
  // For testing, you can hardcode it here:
  // strcpy(command, "ON");
  delay(100);
}

// Send the current command to the Pi when requested
void sendCommand() {
  Wire.write((const char*)command, COMMAND_LEN);
}