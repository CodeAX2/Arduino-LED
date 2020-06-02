#include "GradientColorMode.h"
#include "Handler.h"

GradientColorMode::GradientColorMode() : ArduinoMode(ArduinoMode::ModeID::GRADIENT_COLOR_MODE) {}

void GradientColorMode::handleData(byte data) {

	if (doneReadingData) {
		Handler::getModeByID(ArduinoMode::ModeID::NO_MODE)->handleData(data);
		return;
	}

	if (currentByteInStream == 0) {
		numGradientColors = data;
	} else {

		int relativeByteInStream = (currentByteInStream - 1) % 5;
		int currentGradientDataIndex = (currentByteInStream - 1) / 5;

		if (currentGradientDataIndex >= 8)
			return;

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

			int redChannel, greenChannel, blueChannel;
			Handler::getRGBChannels(&redChannel, &greenChannel, &blueChannel);

			gradientColors[currentGradientDataIndex] =
				ChannelRGB(curGradientRed, curGradientGreen, curGradientBlue,
					redChannel, greenChannel, blueChannel);

			fadeTimes[currentGradientDataIndex] = curFadeTime;

			if (currentGradientDataIndex == numGradientColors - 1) {
				doneReadingData = true;
			}
			break;
		}


	}

	currentByteInStream++;

}
void GradientColorMode::loop() {

	if (!doneReadingData) return;

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

		rgb_color* colors = new rgb_color[Handler::getNumLEDs()];

		for (uint16_t i = 0; i < Handler::getNumLEDs(); i++) {
			colors[i] = curColor.getColorForLED();
		}

		Handler::getLEDStrip()->write(colors, Handler::getNumLEDs());
		delete[] colors;

		tickCounter++;
		prevUpdate = millis();
	}

}

void GradientColorMode::activate() {
	ArduinoMode::activate();
	doneReadingData = false;
	tickCounter = 0;
}