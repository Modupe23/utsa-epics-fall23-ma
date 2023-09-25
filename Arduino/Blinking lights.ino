int led_pin1 = 13;
int led_pin2 = 12;
int blink_duration1 = 100;

void setup() {
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);

}

void loop() {
  
  digitalWrite (led_pin1,HIGH);
  delay (blink_duration1);
  digitalWrite (led_pin1,LOW);
  delay (blink_duration1);

  digitalWrite (led_pin2,HIGH);
  delay (blink_duration1);
  digitalWrite (led_pin2,LOW);

}