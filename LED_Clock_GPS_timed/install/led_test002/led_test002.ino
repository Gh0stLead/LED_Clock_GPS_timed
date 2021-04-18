#include "Adafruit_NeoPixel.h"
#define PIN       6
#define NUMPIXELS 60

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  pixels.setPixelColor(3,pixels.Color(150,0,0));
  pixels.show();

}

void loop() {
  // put your main code here, to run repeatedly:

}
