#include "ChannelRGB.h"

ChannelRGB::ChannelRGB() {
  red = 0, green = 0, blue = 0;
  redChannel = 0, greenChannel = 1, blueChannel = 2;
}

ChannelRGB::ChannelRGB(int red, int green, int blue) {
  this->red = red;
  this->blue = blue;
  this->green = green;
}

ChannelRGB::ChannelRGB(int red, int green, int blue, int redChannel, int greenChannel, int blueChannel) {
  this->red = red;
  this->blue = blue;
  this->green = green;
  this->redChannel = redChannel;
  this->blueChannel = blueChannel;
  this->greenChannel = greenChannel;
}

ChannelRGB::ChannelRGB(const ChannelRGB& color){
  red = color.red;
  green = color.green;
  blue = color.blue;

  redChannel = color.redChannel;
  greenChannel = color.greenChannel;
  blueChannel = color.blueChannel;
}

void ChannelRGB::setColor(int red, int green, int blue) {
  this->red = red;
  this->blue = blue;
  this->green = green;
}

void ChannelRGB::setRed(int red) {
  this->red = red;
}

void ChannelRGB::setGreen(int green) {
  this->green = green;
}

void ChannelRGB::setBlue(int blue) {
  this->blue = blue;
}

int ChannelRGB::getRed() {
  return red;
}

int ChannelRGB::getGreen() {
  return green;
}

int ChannelRGB::getBlue() {
  return blue;
}

void ChannelRGB::setColorChannels(int redChannel, int greenChannel, int blueChannel) {
  this->redChannel = redChannel;
  this->blueChannel = blueChannel;
  this->greenChannel = greenChannel;
}

void ChannelRGB::setRedChannel(int channel) {
  redChannel = channel;
}

void ChannelRGB::setGreenChannel(int channel) {
  greenChannel = channel;
}

void ChannelRGB::setBlueChannel(int channel) {
  blueChannel = channel;
}

int ChannelRGB::getRedChannel() {
  return redChannel;
}

int ChannelRGB::getGreenChannel() {
  return greenChannel;
}

int ChannelRGB::getBlueChannel() {
  return blueChannel;
}

rgb_color ChannelRGB::getColorForLED() {
  int color0, color1, color2;

  switch (redChannel) {
    case 0:
      color0 = red;
      break;
    case 1:
      color1 = red;
      break;
    case 2:
      color2 = red;
      break;
  }

  switch (greenChannel) {
    case 0:
      color0 = green;
      break;
    case 1:
      color1 = green;
      break;
    case 2:
      color2 = green;
      break;
  }

  switch (blueChannel) {
    case 0:
      color0 = blue;
      break;
    case 1:
      color1 = blue;
      break;
    case 2:
      color2 = blue;
      break;
  }

  return rgb_color(color0, color1, color2);

}
