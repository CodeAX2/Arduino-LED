#include "ChannelRGB.h"
#include <PololuLedStrip.h>
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60
rgb_color colors[LED_COUNT];

const rgb_color OFF_COLOR(0, 0, 0);
ChannelRGB baseColor;

void setup() {

  baseColor.setColorChannels(0, 2, 1); // RBG

}

int index = 0;
const int MAX_INDEX = 9;
const int DELAY = 100;

void loop() {

  ChannelRGB colorA(baseColor);
  colorA.setBlue(255);

  ChannelRGB colorB(baseColor);
  colorB.setGreen(255);


  // Update the colors buffer.
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    if (i == index)
      colors[i] = colorA.getColorForLED();
    else if ((i + 1) % MAX_INDEX == index)
      colors[i] = colorB.getColorForLED();
    else
      colors[i] = OFF_COLOR;
  }

  // Write to the LED strip.
  ledStrip.write(colors, LED_COUNT);

  index++;
  if (index > MAX_INDEX) {
    index = 0;
  }
  delay(DELAY);


}
