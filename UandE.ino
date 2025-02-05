#include "FastLED.h"

CRGB leds1[256];//Panel #1
CRGB leds2[256];//Panel #2

void setup() {  FastLED.setBrightness(8); }

void loop() {
FastLED.addLeds<WS2812B, 6, GRB>(leds1, 256); U();
FastLED.show();
FastLED.addLeds<WS2812B, 7, GRB>(leds2, 256); E();
FastLED.show();


}
