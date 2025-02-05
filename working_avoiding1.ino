#include <IRremote.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <IRremote.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <Servo.h>  // Include the Servo library for controlling the servo motor
Servo Myservo;  // Create a Servo object named Myservo

// Define pin numbers for ultrasonic sensor, I2C communication, and motor control
#define trigPin 12           // Trig Pin Of HC-SR04
#define echoPin 13           // Echo Pin Of HC-SR04
#define SCL_Pin A5           // Set clock pin to A5
#define SDA_Pin A4           // Set data pin to A4
#define ML_Ctrl 4            // Define direction control pin of B motor
#define ML_PWM 5             // Define PWM control pin of B motor
#define MR_Ctrl 2            // Define direction control pin of A motor
#define MR_PWM 6             // Define PWM control pin of A motor

long duration, distance;  // Declare variables to store duration and distance

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  pinMode(ML_Ctrl, OUTPUT);  // Set Motor Control Pins As OUTPUT
  pinMode(ML_PWM, OUTPUT);   // Set PWM pins for motor control as OUTPUT
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  pinMode(trigPin, OUTPUT);  // Set Trig Pin As OUTPUT to transmit waves
  pinMode(echoPin, INPUT);   // Set Echo Pin As INPUT to receive reflected waves
  Myservo.attach(10);  // Attach the servo motor to pin 10
}

void loop() {
  digitalWrite(trigPin, LOW);  // Send a low pulse to trigPin
  delayMicroseconds(2);   // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Send a high pulse to trigPin to transmit waves for 10 microseconds
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);  // Measure the duration of the pulse received on echoPin
  distance = duration / 58.2;   // Calculate the distance based on the duration
  Serial.println(distance);  // Print the distance to the serial monitor
  delay(10);  // Wait for a short time
  
  // Check if the distance is greater than 90 cm (absence of obstacle)
  if (distance > 80) {  
    Myservo.write(90);  // Set the servo motor to a neutral position
    digitalWrite(ML_Ctrl, HIGH);  // Set motor control pins to move forward
    analogWrite(ML_PWM, 220);  // Apply PWM signal to control motor speed
    digitalWrite(MR_Ctrl, HIGH);
    analogWrite(MR_PWM, 190);
  }
  // Check if the distance is less than 70 cm and greater than 0 (presence of obstacle)
   else if ((distance < 50) && (distance > 0)) {
    analogWrite(ML_PWM, 0);  // Stop the left motor
    analogWrite(MR_PWM, 0);   // Stop the right motor
    delay(100);  // Delay for stability
    
    // Perform avoidance maneuver using the servo motor
    Myservo.write(0);  // Rotate servo motor to the left
    delay(500);
    Myservo.write(180);  // Rotate servo motor to the right
    delay(500);
    Myservo.write(90);   // Set servo motor to a neutral position
    delay(500);
    
    // Move the robot backward after avoiding the obstacle
    digitalWrite(ML_Ctrl, LOW);   // Set motor control pins for left motor to move backward
    analogWrite(ML_PWM, 220);      // Apply PWM signal to control left motor speed
    digitalWrite(MR_Ctrl, LOW);   // Set motor control pins for right motor to move backward
    analogWrite(MR_PWM, 190);      // Apply PWM signal to control right motor speed

    delay(500);  // Delay for stability

    // Move the robot right to avoid the obstacle
    digitalWrite(ML_Ctrl, HIGH);   // Set motor control pins for left motor to move forward
    digitalWrite(MR_Ctrl, LOW);   // Set motor control pins for right motor to turn right
    delay(500);  // Delay for stability
    
    // Resume forward motion after avoiding the obstacle
    digitalWrite(ML_Ctrl, HIGH);   // Set motor control pins for left motor to move forward
    digitalWrite(MR_Ctrl, HIGH);   // Set motor control pins for right motor to move forward
    delay(500);  // Delay for stability
  }
}
