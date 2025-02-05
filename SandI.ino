#include "FastLED.h"
CRGB leds3[256];
CRGB leds6[256];//Panel #2

void setup() {  FastLED.setBrightness(8);}

void loop() {
FastLED.addLeds<WS2812B, 6, GRB>(leds3, 256); S();
FastLED.show();
FastLED.addLeds<WS2812B, 7, GRB>(leds6, 256); I();
FastLED.show();}
