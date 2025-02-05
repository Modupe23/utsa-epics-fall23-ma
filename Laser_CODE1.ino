// Code for controlling two lasers without blinking using Arduino
// Author: Oyvind N. Dahl
// Website: https://www.build-electronic-circuits.com/

void setup() {
  pinMode(1, OUTPUT); // Set pin 1 as output for first laser
  pinMode(2, OUTPUT); // Set pin 2 as output for second laser
}

void loop() {
  digitalWrite(1, HIGH); // Turn on first laser
  digitalWrite(2, HIGH); // Turn on second laser
}
