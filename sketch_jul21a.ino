#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it.
#endif

BluetoothSerial SerialBT;

// Define the digital pin where the VIBRATION sensor's digital output is connected
const int VIBRATION_SENSOR_PIN = 15; // Using Digital Pin 15

// Variables for VIBRATION detection state and debouncing
int lastVIBRATIONState = HIGH;        // Stores the previous state of the VIBRATION sensor
unsigned long lastDebounceTime = 0; // The last time the output pin was toggled
const unsigned long debounceDelay = 200; // Debounce time in milliseconds (adjust as needed)

void setup() {
  Serial.begin(115200); // Initialize Serial for debugging messages (view via USB)
  SerialBT.begin("ESP32_VIBRATION"); // Initialize Bluetooth with a recognizable name
  Serial.println("ESP32 Bluetooth started! Waiting for phone connection...");

  // Set the VIBRATION sensor pin as an input
  pinMode(VIBRATION_SENSOR_PIN, INPUT);
}

void loop() {
  // Read the current state of the VIBRATION sensor's digital output
  int currentVIBRATIONState = digitalRead(VIBRATION_SENSOR_PIN);

  // Check if the VIBRATION sensor state has changed (from LOW to HIGH)
  // and if enough time has passed for debouncing.
  if (currentVIBRATIONState == LOW && lastVIBRATIONState == HIGH) {
    // Check for debounce time
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // VIBRATION detected! Send message over Bluetooth.
      // SerialBT.println() automatically appends a carriage return (\r) and newline (\n).
      // The newline (\n) is crucial for App Inventor's ReceiveText number -1.
      SerialBT.println("VIBRATION detected!");

      Serial.println("Sent via Bluetooth: VIBRATION detected!"); // Debugging message on Serial Monitor
      
      lastDebounceTime = millis(); // Update the last time a detection occurred
    }
  }

  // Save the current state for the next loop iteration
  lastVIBRATIONState = currentVIBRATIONState;

  // Small delay to prevent the loop from running too fast,
  // which can be useful for stability.
  delay(10);
}