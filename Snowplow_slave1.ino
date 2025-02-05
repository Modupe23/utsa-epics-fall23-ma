#include <Servo.h>
#include <SoftwareSerial.h>

Servo leftServo;  // Create a servo object for the left side of the snowplow
Servo rightServo; // Create a servo object for the right side of the snowplow

const int leftServoPin = 9;    // Pin connected to the left servo
const int rightServoPin = 10;  // Pin connected to the right servo
const int leftLaserPin = 1;    // Pin connected to the left laser
const int rightLaserPin = 2;   // Pin connected to the right laser

SoftwareSerial bluetooth(13, 12); // RX, TX

bool bluetoothInitialized = false; // Flag to track Bluetooth initialization

void setup() {
  Serial.begin(9600); // Serial monitor
  Serial.println("Serial monitor initialized.");

  leftServo.attach(leftServoPin);   // Attach the left servo to its pin
  rightServo.attach(rightServoPin); // Attach the right servo to its pin
  pinMode(leftLaserPin, OUTPUT);    // Set the left laser pin as output
  pinMode(rightLaserPin, OUTPUT);   // Set the right laser pin as output

  Serial.println("Setup completed.");
}

void loop() {
  // Initialize Bluetooth module (once)
  if (!bluetoothInitialized) {
    bluetooth.begin(9600); // Bluetooth module
    Serial.println("Bluetooth module initialized.");
    bluetoothInitialized = true;
  }

  // Read data from Bluetooth module
  if (bluetooth.available() >= 2) {
    int joystickX = bluetooth.read(); // Read joystick X-axis value
    int joystickY = bluetooth.read(); // Read joystick Y-axis value

    Serial.print("Joystick X-axis: ");
    Serial.println(joystickX);
    Serial.print("Joystick Y-axis: ");
    Serial.println(joystickY);

    // Map joystick X-axis value to servo positions
    int leftAngle = map(joystickX, 0, 255, 0, 180);
    int rightAngle = map(joystickX, 0, 255, 180, 0);

    Serial.print("Left servo angle: ");
    Serial.println(leftAngle);
    Serial.print("Right servo angle: ");
    Serial.println(rightAngle);

    // Move servos according to joystick position
    leftServo.write(leftAngle);
    rightServo.write(rightAngle);

    // Toggle lasers based on joystick Y-axis value (or button state, adjust as needed)
    if (joystickY > 128) { // Example condition, adjust as needed
      digitalWrite(leftLaserPin, HIGH);
      digitalWrite(rightLaserPin, HIGH);
      Serial.println("Lasers turned ON.");
    } else {
      digitalWrite(leftLaserPin, LOW);
      digitalWrite(rightLaserPin, LOW);
      Serial.println("Lasers turned OFF.");
    }
  }
}
