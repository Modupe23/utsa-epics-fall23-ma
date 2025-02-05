#include "FastLED.h"
CRGB leds2[256];//Panel #2
CRGB leds7[256];

void setup() {  FastLED.setBrightness(8);}

void loop() {
FastLED.addLeds<WS2812B, 6, GRB>(leds2, 256); T();
FastLED.show();
FastLED.addLeds<WS2812B, 7, GRB>(leds7, 256); P();
FastLED.show();}
