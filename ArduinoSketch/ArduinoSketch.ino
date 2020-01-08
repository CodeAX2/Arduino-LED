#include "ChannelRGB.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <PololuLedStrip.h>

const rgb_color OFF_COLOR(0, 0, 0);
const int NO_MODE = -1;
const int SETUP_MODE = 0;
const int ALL_STATIC_COLOR_MODE = 1;
const int GRADIENT_COLOR_MODE = 2;
const int INDIVIDUAL_STATIC_COLOR_MODE = 3;
const int AUDIO_MODE = 4;
const int STARTUP_DATA_SET_MODE = 5;

int currentMode = NO_MODE;
PololuLedStrip<12> ledStrip;

// Setup Mode Data and Defaults
int numLed = 60;
int redChannel = 0;
int greenChannel = 1;
int blueChannel = 2;

// Gradient Mode Data
ChannelRGB gradientColors[8];
unsigned int fadeTimes[8];
int numGradientColors = 0;
bool isGradient = false;
unsigned long prevUpdate;
int curColorIndex = 0;
unsigned long curColorFadeTimer = 0;
int tickCounter = 0;
int strobeSpeed = 0;

// Audio Mode Data
ChannelRGB audioColors[8];
int numAudioColors = 0;
bool isAudio = false;
unsigned int audioLevel = 0;
int audioColorIndex = 0;
const int levelsToSave = 15;
unsigned int prevLevels[levelsToSave];
int ticksSinceLastAudioPeak;
int prevBrightness = 0;
int maxBrightnessDelta = 10;
int prevBrightnessDelta = 0;
int prevPeakValue = 0;
int prevMinValue = 1000;

byte defaultStartupData[] = {0, 60, 0, 0, 2, 1, 1, 0, 200, 75};

void setup() {

  prevUpdate = millis();
  Serial.begin(9600);

  byte numStartupBytes = EEPROM.read(0);

  if (numStartupBytes == 0 || numStartupBytes > 64) {

    for (int i = 0; i < sizeof(defaultStartupData) / sizeof(byte); i++) {
      handleData(defaultStartupData[i]);
    }
    EEPROM.update(0, 0);
  } else {
    for (int i = 0; i < numStartupBytes; i++) {
      byte currentByte = EEPROM.read(1 + i);
      handleData(currentByte);
    }
  }
}

void loop() {

  if (Serial.available() > 0) {
    // Recieved data over serial
    byte incomingByte = Serial.read();
    handleData(incomingByte);
  }

  if (isGradient) {
    if (millis() - prevUpdate >= 1000 / 60) {

      curColorFadeTimer += millis() - prevUpdate;
      int fadeTime = fadeTimes[curColorIndex];

      if (curColorFadeTimer > fadeTime) {
        curColorFadeTimer = 0;
        curColorIndex = (curColorIndex + 1) % numGradientColors;
        fadeTime = fadeTimes[curColorIndex];
      }

      ChannelRGB colorA = gradientColors[curColorIndex];
      ChannelRGB colorB =
          gradientColors[(curColorIndex + 1) % numGradientColors];

      ChannelRGB curColor(colorA);

      float percentTime = (float)curColorFadeTimer / fadeTime;
      int ar = colorA.getRed();
      int ag = colorA.getGreen();
      int ab = colorA.getBlue();

      int br = colorB.getRed();
      int bg = colorB.getGreen();
      int bb = colorB.getBlue();

      curColor.setRed(ar + (br - ar) * percentTime);
      curColor.setGreen(ag + (bg - ag) * percentTime);
      curColor.setBlue(ab + (bb - ab) * percentTime);

      if (strobeSpeed != 0 &&
          tickCounter % (60 / strobeSpeed) < 60 / strobeSpeed / 2) {
        curColor.setRed(0);
        curColor.setBlue(0);
        curColor.setGreen(0);
      }

      rgb_color colors[numLed];
      for (uint16_t i = 0; i < numLed; i++) {
        colors[i] = curColor.getColorForLED();
      }
      ledStrip.write(colors, numLed);

      tickCounter++;
      prevUpdate = millis();
    }
  } else if (isAudio) {

    if (millis() - prevUpdate >= 1000 / 60) {

      ticksSinceLastAudioPeak++;

      for (int i = 1; i < levelsToSave; i++) {
        prevLevels[i - 1] = prevLevels[i];
      }

      prevLevels[levelsToSave - 1] = audioLevel;

      if (audioLevel < prevMinValue)
        prevMinValue = audioLevel;

      float avgChange = (audioLevel - prevLevels[0]) / (levelsToSave);
      float avgSecondChange = 0;

      for (int i = 2; i < levelsToSave; i++) {
        float dif1 = prevLevels[i] - prevLevels[i - 1];
        float dif2 = prevLevels[i - 1] - prevLevels[i - 2];
        avgSecondChange += (dif1 - dif2);
      }

      avgSecondChange /= (float)(levelsToSave - 2); // Second derivative

      bool changedColor = false;

      if (((avgSecondChange < 0 && avgChange <= 10 && avgChange >= 0) ||
           (avgChange >= 100 && ticksSinceLastAudioPeak >= 60)) &&
          (ticksSinceLastAudioPeak >= 45 ||
           prevMinValue < prevPeakValue - 200 ||
           prevMinValue < audioLevel - 200) &&
          ticksSinceLastAudioPeak >= 20) {

        if (ticksSinceLastAudioPeak >= 45)
          audioColorIndex = (audioColorIndex + 1) % numAudioColors;
        changedColor = true;
        ticksSinceLastAudioPeak = 0;
        prevPeakValue = audioLevel;
        prevMinValue = 1000;
      }

      float multiplier = (4.75f / 5.f) * avgChange + (10.5f / 2.f);

      if (multiplier < .5)
        multiplier = .5;
      if (multiplier > 10)
        multiplier = 10;

      // int curBrightness = ((float)audioLevel / 1000 * 255);
      int curBrightness = 60 - ticksSinceLastAudioPeak *
                                   ((float)675 / (float)(audioLevel + 50));
      if (curBrightness > 60)
        curBrightness = 60;
      if (curBrightness < 0)
        curBrightness = 0;

      curBrightness = map(curBrightness, 0, 60, 0, 255);

      if (curBrightness > 255)
        curBrightness = 255;
      if (curBrightness < 0)
        curBrightness = 0;

      if (abs(curBrightness - prevBrightness - prevBrightnessDelta) >= 20) {
        maxBrightnessDelta = 5;
      } else {
        maxBrightnessDelta = 10;
      }

      if (abs(curBrightness - prevBrightness) > maxBrightnessDelta &&
          !changedColor) {

        if (curBrightness < prevBrightness) {
          curBrightness = prevBrightness - maxBrightnessDelta;
        } else {
          curBrightness = prevBrightness + maxBrightnessDelta;
        }
      }

      ChannelRGB curColor(audioColors[audioColorIndex]);
      curColor = setBrightness(curColor, (float)curBrightness / 255);

      // curColor.setRed(curBrightness);
      // curColor.setGreen(curBrightness);
      // curColor.setBlue(curBrightness);

      rgb_color colors[numLed];
      for (uint16_t i = 0; i < numLed; i++) {
        colors[i] = curColor.getColorForLED();
      }
      ledStrip.write(colors, numLed);

      prevUpdate = millis();
      prevBrightnessDelta = curBrightness - prevBrightness;
      prevBrightness = curBrightness;
    }
  }
}

int currentByteInStream = 0;

void handleData(byte data) {

  switch (currentMode) {
  case NO_MODE:
    currentByteInStream = 0;
    currentMode = data;
    break;

  case SETUP_MODE:
    setupModeData(data);
    break;

  case ALL_STATIC_COLOR_MODE:
    isGradient = false;
    isAudio = false;
    allStaticColorModeData(data);
    break;

  case GRADIENT_COLOR_MODE:
    isAudio = false;
    gradientColorModeData(data);
    break;

  case INDIVIDUAL_STATIC_COLOR_MODE:
    isAudio = false;
    isGradient = false;
    individualStaticColorModeData(data);
    break;

  case AUDIO_MODE:
    isGradient = false;
    audioModeData(data);
    break;

  case STARTUP_DATA_SET_MODE:
    startupDataSetModeData(data);
    break;
  }
}

void setupModeData(byte data) {

  switch (currentByteInStream) {
  case 0:
    numLed = data;
    break;
  case 1:
    numLed += 256 * (int)data;
    break;
  case 2:
    redChannel = data;
    break;
  case 3:
    greenChannel = data;
    break;
  case 4:
    blueChannel = data;
    currentMode = NO_MODE;
    break;
  }

  currentByteInStream++;
}

int staticRed = 0;
int staticGreen = 0;
int staticBlue = 0;
void allStaticColorModeData(byte data) {

  switch (currentByteInStream) {
  case 0:
    staticRed = data;
    break;
  case 1:
    staticGreen = data;
    break;
  case 2:
    staticBlue = data;

    ChannelRGB color(staticRed, staticGreen, staticBlue, redChannel,
                     greenChannel, blueChannel);
    rgb_color colors[numLed];
    for (uint16_t i = 0; i < numLed; i++) {
      colors[i] = color.getColorForLED();
    }
    ledStrip.write(colors, numLed);
    currentMode = NO_MODE;
    break;
  }

  currentByteInStream++;
}

int curGradientRed = 0;
int curGradientGreen = 0;
int curGradientBlue = 0;
unsigned int curFadeTime = 0;
void gradientColorModeData(byte data) {
  switch (currentByteInStream) {
  case 0:
    numGradientColors = data;
    break;
  default:

    int relativeByteInStream = (currentByteInStream - 1) % 5;
    int currentGradientDataIndex = (currentByteInStream - 1) / 5;
    if (currentGradientDataIndex >= 8)
      break;
    switch (relativeByteInStream) {
    case 0:
      curGradientRed = data;
      break;
    case 1:
      curGradientGreen = data;
      break;
    case 2:
      curGradientBlue = data;
      break;
    case 3:
      curFadeTime = data;
      break;
    case 4:
      curFadeTime += 256 * (unsigned int)data;
      gradientColors[currentGradientDataIndex] =
          ChannelRGB(curGradientRed, curGradientGreen, curGradientBlue,
                     redChannel, greenChannel, blueChannel);
      fadeTimes[currentGradientDataIndex] = curFadeTime;

      if (currentGradientDataIndex == numGradientColors - 1) {
        isGradient = true;
        currentMode = NO_MODE;
      }
      break;
    }

    break;
  }

  currentByteInStream++;
}

int numLedsToUpdate = 0;
int currentLedID = 0;
int currentLedRed = 0, currentLedGreen = 0, currentLedBlue = 0;
rgb_color *individualLedStripData;

void individualStaticColorModeData(byte data) {

  switch (currentByteInStream) {
  case 0:
    numLedsToUpdate = data;
    break;
  case 1:
    numLedsToUpdate += 256 * data;
    individualLedStripData = new rgb_color[numLed];
    for (int i = 0; i < numLed; i++) {
      individualLedStripData[i] = rgb_color(0, 0, 0);
    }
    break;
  default:
    int relativeByteInStream = (currentByteInStream - 2) % 5;
    int currentColorDataIndex = (currentByteInStream - 2) / 5;
    switch (relativeByteInStream) {
    case 0:
      currentLedID = data;
      break;
    case 1:
      currentLedID += 256 * data;
      break;
    case 2:
      currentLedRed = data;
      break;
    case 3:
      currentLedGreen = data;
      break;
    case 4:
      currentLedBlue = data;

      ChannelRGB color(currentLedRed, currentLedGreen, currentLedBlue,
                       redChannel, greenChannel, blueChannel);
      individualLedStripData[currentLedID] = color.getColorForLED();
      if (currentColorDataIndex == numLedsToUpdate - 1) {
        ledStrip.write(individualLedStripData, numLed);
        delete[] individualLedStripData;
        currentMode = NO_MODE;
      }

      break;
    }

    break;
  }

  currentByteInStream++;
}

int curAudioRed = 0;
int curAudioGreen = 0;
int curAudioBlue = 0;
int audioByteStreamBegin = 0;
unsigned int tempAudioLevel = 0;
void audioModeData(byte data) {
  switch (currentByteInStream) {
  case 0:
    numAudioColors = data;
    break;
  default:

    if (!isAudio) {
      int relativeByteInStream = (currentByteInStream - 1) % 3;
      int currentAudioDataIndex = (currentByteInStream - 1) / 3;
      if (currentAudioDataIndex >= 8)
        break;
      switch (relativeByteInStream) {
      case 0:
        curAudioRed = data;
        break;
      case 1:
        curAudioGreen = data;
        break;
      case 2:
        curAudioBlue = data;
        audioColors[currentAudioDataIndex] =
            ChannelRGB(curAudioRed, curAudioGreen, curAudioBlue, redChannel,
                       greenChannel, blueChannel);

        if (currentAudioDataIndex == numAudioColors - 1) {
          isAudio = true;
          audioByteStreamBegin = currentByteInStream;
        }
        break;
      }
    } else {

      if (currentByteInStream - audioByteStreamBegin > 0) {
        if ((currentByteInStream - audioByteStreamBegin) % 2 == 1) {
          tempAudioLevel = (unsigned int)data;
        } else {
          tempAudioLevel += 256 * (unsigned int)data;
          audioLevel = tempAudioLevel;
          if (tempAudioLevel == 65535) {
            isAudio = false;
            currentMode = NO_MODE;
          }
        }
      }
    }

    break;
  }

  currentByteInStream++;
}

int numStoredBytes = 0;

void startupDataSetModeData(byte data) {

  switch (currentByteInStream) {
  case 0:
    numStoredBytes = data;
    if (data <= 64)
      EEPROM.update(0, data);
    break;
  default:
    if (currentByteInStream <= 65) {
      EEPROM.update(currentByteInStream, data);
      if (currentByteInStream == numStoredBytes + 1) {
        currentMode = NO_MODE;
      }
    }
    break;
  }

  currentByteInStream++;
}

ChannelRGB setBrightness(ChannelRGB baseColor, float brightness) {

  float fHue, sat, val;
  float r = baseColor.getRed() / (float)255;
  float g = baseColor.getGreen() / (float)255;
  float b = baseColor.getBlue() / (float)255;

  RGBtoHSV(r, g, b, fHue, sat, val);

  val = brightness;
  int hue = fHue;

  hue %= 360;
  while (hue < 0)
    hue += 360;

  if (sat < 0.f)
    sat = 0.f;
  if (sat > 1.f)
    sat = 1.f;

  if (val < 0.f)
    val = 0.f;
  if (val > 1.f)
    val = 1.f;

  int h = hue / 60;
  float f = float(hue) / 60 - h;
  float p = val * (1.f - sat);
  float q = val * (1.f - sat * f);
  float t = val * (1.f - sat * (1 - f));

  switch (h) {
  default:
  case 0:
  case 6:
    return ChannelRGB(val * 255, t * 255, p * 255, redChannel, greenChannel,
                      blueChannel);
  case 1:
    return ChannelRGB(q * 255, val * 255, p * 255, redChannel, greenChannel,
                      blueChannel);
  case 2:
    return ChannelRGB(p * 255, val * 255, t * 255, redChannel, greenChannel,
                      blueChannel);
  case 3:
    return ChannelRGB(p * 255, q * 255, val * 255, redChannel, greenChannel,
                      blueChannel);
  case 4:
    return ChannelRGB(t * 255, p * 255, val * 255, redChannel, greenChannel,
                      blueChannel);
  case 5:
    return ChannelRGB(val * 255, p * 255, q * 255, redChannel, greenChannel,
                      blueChannel);
  }
}

void RGBtoHSV(float &fR, float &fG, float fB, float &fH, float &fS, float &fV) {
  float fCMax = max(max(fR, fG), fB);
  float fCMin = min(min(fR, fG), fB);
  float fDelta = fCMax - fCMin;

  if (fDelta > 0) {
    if (fCMax == fR) {
      fH = 60 * (fmod(((fG - fB) / fDelta), 6));
    } else if (fCMax == fG) {
      fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if (fCMax == fB) {
      fH = 60 * (((fR - fG) / fDelta) + 4);
    }

    if (fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }

    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }

  if (fH < 0) {
    fH = 360 + fH;
  }
}
