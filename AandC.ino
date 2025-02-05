#include "FastLED.h"

CRGB leds[256];//Panel #1
CRGB leds2[256];//Panel #2

void setup() { FastLED.setBrightness(8); }

void loop() {

FastLED.addLeds<WS2812B, 6, GRB>(leds, 256); A();
FastLED.show();
FastLED.addLeds<WS2812B, 7, GRB>(leds2, 256); C();
FastLED.show();
}
