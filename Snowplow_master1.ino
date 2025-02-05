#include <SoftwareSerial.h>

SoftwareSerial bluetooth(13, 12); // RX, TX

void setup() {
  Serial.begin(9600); // Serial monitor
  bluetooth.begin(9600); // Bluetooth module
}

void loop() {
  // Read joystick values
  int joystickX = map(analogRead(A0), 0, 1023, 0, 255);
  int joystickY = map(analogRead(A1), 0, 1023, 0, 255);
  int buttonState = digitalRead(7); // Assuming button connected to pin 7

  // Send joystick data over Bluetooth
  bluetooth.write(joystickX);
  bluetooth.write(joystickY);
  bluetooth.write(buttonState);

  delay(100); // Adjust delay as needed
}
