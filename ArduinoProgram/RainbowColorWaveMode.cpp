#include "RainbowColorWaveMode.h"
#include "Handler.h"
#include "Utils.h"

RainbowColorWaveMode::RainbowColorWaveMode() : ArduinoMode(ArduinoMode::ModeID::RAINBOW_COLOR_WAVE_MODE) {}

void RainbowColorWaveMode::handleData(byte data) {

	if (doneReadingData) {
		Handler::getModeByID(ArduinoMode::ModeID::NO_MODE)->handleData(data);
		return;
	}

	switch (currentByteInStream) {
	case 0:
		rainbowWaveMSDelay = data;
		break;
	case 1:
		rainbowWaveMSDelay += 256 * data;
		break;
	case 2:
		rainbowWaveLength = data;
		break;
	case 3:
		rainbowWaveLength += 256 * data;
		doneReadingData = true;
		msSinceRainbowWaveStart = 0;
		break;
	}

	currentByteInStream++;

}
void RainbowColorWaveMode::loop() {

	if (!doneReadingData) return;

	rgb_color* colors = new rgb_color[Handler::getNumLEDs()];

	float huePerLED = 1.0 / rainbowWaveLength;
	for (uint16_t i = 0; i < Handler::getNumLEDs(); i++) {
		int rainR = 0, rainG = 0, rainB = 0;
		float hue = (i % rainbowWaveLength) / (float)(rainbowWaveLength);
		hue += ((float)msSinceRainbowWaveStart / rainbowWaveMSDelay) * huePerLED;
		Utils::HSVtoRGB(hue, 1, 1, rainR, rainG, rainB);

		int rChannel, gChannel, bChannel;
		Handler::getRGBChannels(&rChannel, &gChannel, &bChannel);

		colors[i] = ChannelRGB(rainR, rainG, rainB, rChannel, gChannel, bChannel).getColorForLED();
	}

	Handler::getLEDStrip()->write(colors, Handler::getNumLEDs());

	msSinceRainbowWaveStart += millis() - prevUpdate;
	prevUpdate = millis();
	delete[] colors;


}

void RainbowColorWaveMode::activate() {
	ArduinoMode::activate();
	doneReadingData = false;
}