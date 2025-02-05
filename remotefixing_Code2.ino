#include <IRremote.h>
#include <HCSR04.h>
#include <Servo.h> // Add this include for the Servo library

unsigned char start01[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char front_matrix[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x12, 0x09, 0x12, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char back_matrix[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x48, 0x90, 0x48, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char left_matrix[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x10, 0x44, 0x28, 0x10, 0x44, 0x28, 0x10, 0x00};
unsigned char right_matrix[] = {0x00, 0x10, 0x28, 0x44, 0x10, 0x28, 0x44, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char STOP01[] = {0x7c, 0x40, 0x7c, 0x00, 0x04, 0x7c, 0x04, 0x00, 0x5c, 0x54, 0x74, 0x00, 0x7c, 0x14, 0x14, 0x7c};
unsigned char clear[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define SCL_Pin A5
#define SDA_Pin A4
#define left_ctrl 4            // Define direction control pin of B motor
#define left_pwm 5             // Define PWM control pin of B motor
#define right_ctrl 2            // Define direction control pin of A motor
#define right_pwm 6             // Define PWM control pin of A motor
#define TRIG_PIN 12
#define ECHO_PIN 13

HCSR04 hc_sr04 = HCSR04(TRIG_PIN, ECHO_PIN);
long duration, distance;  // Declare variables to store duration and distance

Servo Myservo;           // Example servo object

const int sensor_l = 11;//define the pin of left line tracking sensor
const int sensor_c = 7;//define the pin of middle line tracking sensor
const int sensor_r = 8;//define the pin of right line tracking sensor
int l_val,c_val,r_val;//define these variables
const int servopin = 10;

IRrecv irrecv(3); // IR Receiver on pin 3
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  servopulse(servopin, 90);
  pinMode(left_ctrl, OUTPUT);
  pinMode(left_pwm, OUTPUT);
  pinMode(right_ctrl, OUTPUT);
  pinMode(right_pwm, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);  // Set Trig Pin As OUTPUT to transmit waves
  pinMode(ECHO_PIN, INPUT);   // Set Echo Pin As INPUT to receive reflected waves
  pinMode(sensor_l, INPUT);
  pinMode(sensor_c, INPUT);
  pinMode(sensor_r, INPUT);
  pinMode(SCL_Pin, OUTPUT);
  pinMode(SDA_Pin, OUTPUT);
  matrix_display(clear);
  matrix_display(start01);
  Myservo.attach(10); // Attach the servo motor to pin 10
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long IR_val = results.value;
    Serial.println(IR_val, HEX);
    switch (IR_val) {
      case 0xFF629D: front(); matrix_display(front_matrix); break; // Forward button
      case 0xFFA857: back(); matrix_display(back_matrix); break;   // Backward button
      case 0xFF22DD: left(); matrix_display(left_matrix); break;   // Left button
      case 0xFFC23D: right(); matrix_display(right_matrix); break; // Right button
      case 0xFF02FD: Stop(); matrix_display(STOP01); break;        // Stop button
      case 0xFF52AD: 
        avoid(); // Call the avoid function when button is pressed
        break;                         
      case 0xFF42BD: tracking(); break; // Button FF42BD triggers tracking mode
      // Add more cases for other buttons if needed
    }
    irrecv.resume(); // Receive the next value
  }

  // Other tasks can be handled here
}

void avoid() {
  // Add a loop for continuous obstacle avoidance
  while (digitalRead(buttonPin) == HIGH) {
    digitalWrite(TRIG_PIN, LOW); // Send a low pulse to trigPin
    delayMicroseconds(2);        // Wait for 2 microseconds
    digitalWrite(TRIG_PIN, HIGH); // Send a high pulse to trigPin to transmit waves for 10 microseconds
    delayMicroseconds(10);
    duration = pulseIn(ECHO_PIN, HIGH); // Measure the duration of the pulse received on echoPin
    distance = duration / 58.2;        // Calculate the distance based on the duration
    Serial.println(distance);          // Print the distance to the serial monitor
    delay(10);                          // Wait for a short time

    // Check if the distance is greater than 80 cm (absence of obstacle)
    if (distance > 80) {  
      Myservo.write(90);  // Set the servo motor to a neutral position
      digitalWrite(left_ctrl, HIGH);  // Set motor control pins to move forward
      analogWrite(left_pwm, 220);  // Apply PWM signal to control motor speed
      digitalWrite(right_ctrl, HIGH);
      analogWrite(right_pwm, 190);
    }
    // Check if the distance is less than 50 cm and greater than 0 (presence of obstacle)
    else if ((distance < 50) && (distance > 0)) {
      analogWrite(left_pwm, 0);  // Stop the left motor
      analogWrite(right_pwm, 0);   // Stop the right motor
      delay(100);  // Delay for stability
      
      // Perform avoidance maneuver using the servo motor
      Myservo.write(0);  // Rotate servo motor to the left
      delay(500);
      Myservo.write(180);  // Rotate servo motor to the right
      delay(500);
      Myservo.write(90);   // Set servo motor to a neutral position
      delay(500);
      
      // Move the robot backward after avoiding the obstacle
      digitalWrite(left_ctrl, LOW);   // Set motor control pins for left motor to move backward
      analogWrite(left_pwm, 220);      // Apply PWM signal to control left motor speed
      digitalWrite(right_ctrl, LOW);   // Set motor control pins for right motor to move backward
      analogWrite(right_pwm, 190);      // Apply PWM signal to control right motor speed

      delay(500);  // Delay for stability

      // Move the robot right to avoid the obstacle
      digitalWrite(left_ctrl, HIGH);   // Set motor control pins for left motor to move forward
      digitalWrite(right_ctrl, LOW);   // Set motor control pins for right motor to turn right
      delay(500);  // Delay for stability
      
      // Resume forward motion after avoiding the obstacle
      digitalWrite(left_ctrl, HIGH);   // Set motor control pins for left motor to move forward
      digitalWrite(right_ctrl, HIGH);   // Set motor control pins for right motor to move forward
      delay(500);  // Delay for stability
    }
  }
}

void tracking() {
  // Add your tracking mode code here
  // The robot follows a black line using three line-tracking sensors (left, center, and right). It adjusts its movement to stay on the line.
  l_val = digitalRead(sensor_l); // Read left sensor
  c_val = digitalRead(sensor_c); // Read center sensor
  r_val = digitalRead(sensor_r); // Read right sensor

  if (c_val == 1) { // If center sensor detects the line
    front();         // Move forward
  } else { // If center sensor doesn't detect the line
    if (l_val == 1 && r_val == 0) { // If only left sensor detects the line
      left();                       // Turn left
    } else if (l_val == 0 && r_val == 1) { // If only right sensor detects the line
      right();                              // Turn right
    } else { // If both left and right sensors detect or don't detect the line
      Stop(); // Stop
    }
  }
}

void servopulse(int servopin, int myangle) {
  for (int i = 0; i < 30; i++) {
    int pulsewidth = (myangle * 11) + 500;
    digitalWrite(servopin, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servopin, LOW);
    delay(20 - pulsewidth / 1000);
  }
}

void front() {
  digitalWrite(left_ctrl, HIGH);
  analogWrite(left_pwm, 220);
  digitalWrite(right_ctrl, HIGH);
  analogWrite(right_pwm, 190);
}

void back() {
  digitalWrite(left_ctrl, LOW);
  analogWrite(left_pwm, 220);
  digitalWrite(right_ctrl, LOW);
  analogWrite(right_pwm, 190);
}

void left() {
  digitalWrite(left_ctrl, LOW);
  analogWrite(left_pwm, 220);
  digitalWrite(right_ctrl, HIGH);
  analogWrite(right_pwm, 190);
}

void right() {
  digitalWrite(left_ctrl, HIGH);
  analogWrite(left_pwm, 220);
  digitalWrite(right_ctrl, LOW);
  analogWrite(right_pwm, 190);
}

void Stop() {
  analogWrite(left_pwm, 0);
  analogWrite(right_pwm, 0);
}

void matrix_display(unsigned char matrix_value[]) {
  IIC_start();
  IIC_send(0xc0);
  for (int i = 0; i < 16; i++) {
    IIC_send(matrix_value[i]);
  }
  IIC_end();
  IIC_start();
  IIC_send(0x8A);
  IIC_end();
}

void IIC_start() {
  digitalWrite(SCL_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin, LOW);
  delayMicroseconds(3);
}

void IIC_send(unsigned char send_data) {
  for (char i = 0; i < 8; i++) {
    digitalWrite(SCL_Pin, LOW);
    delayMicroseconds(3);
    if (send_data & 0x01) {
      digitalWrite(SDA_Pin, HIGH);
    } else {
      digitalWrite(SDA_Pin, LOW);
    }
    delayMicroseconds(3);
    digitalWrite(SCL_Pin, HIGH);
    delayMicroseconds(3);
    send_data = send_data >> 1;
  }
}

void IIC_end() {
  digitalWrite(SCL_Pin, LOW);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin, LOW);
  delayMicroseconds(3);
  digitalWrite(SCL_Pin, HIGH);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin, HIGH);
  delayMicroseconds(3);
}





