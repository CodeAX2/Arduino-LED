#include "TwoColorWaveMode.h"
#include "Handler.h"

TwoColorWaveMode::TwoColorWaveMode() : ArduinoMode(ArduinoMode::ModeID::TWO_COLOR_WAVE_MODE) {}

void TwoColorWaveMode::handleData(byte data) {

	if (doneReadingData) {
		Handler::getModeByID(ArduinoMode::ModeID::NO_MODE)->handleData(data);
		return;
	}

	switch (currentByteInStream) {
	case 0:
		waveColor1.setRed(data);
		break;
	case 1:
		waveColor1.setGreen(data);
		break;
	case 2:
		waveColor1.setBlue(data);
		break;
	case 3:
		waveColor2.setRed(data);
		break;
	case 4:
		waveColor2.setGreen(data);
		break;
	case 5:
		waveColor2.setBlue(data);
		break;
	case 6:
		waveMSDelay = data;
		break;
	case 7:
		waveMSDelay += 256 * data;
		break;
	case 8:
		waveLength = data;
		break;
	case 9:
		waveLength += 256 * data;
		doneReadingData = true;
		msSinceWaveStart = 0;
		break;
	}

	currentByteInStream++;

}

void TwoColorWaveMode::loop() {

	if (!doneReadingData) return;

	rgb_color* colors = new rgb_color[Handler::getNumLEDs()];

	for (uint16_t i = 0; i < Handler::getNumLEDs(); i++) {

		ChannelRGB curColor, nextColor;

		if ((i + msSinceWaveStart / waveMSDelay) % (waveLength * 2) < waveLength) {
			curColor = waveColor1;
		} else {
			curColor = waveColor2;
		}

		if ((i + msSinceWaveStart / waveMSDelay + 1) % (waveLength * 2) < waveLength) {
			nextColor = waveColor1;
		} else {
			nextColor = waveColor2;
		}

		int color2Weight = msSinceWaveStart % waveMSDelay;
		int color1Weight = waveMSDelay - color2Weight;

		int fColorR = ((float)color1Weight * curColor.getRed() + (float)color2Weight * nextColor.getRed()) / (float)waveMSDelay;
		int fColorG = ((float)color1Weight * curColor.getGreen() + (float)color2Weight * nextColor.getGreen()) / (float)waveMSDelay;
		int fColorB = ((float)color1Weight * curColor.getBlue() + (float)color2Weight * nextColor.getBlue()) / (float)waveMSDelay;

		int redChannel, greenChannel, blueChannel;
		Handler::getRGBChannels(&redChannel, &greenChannel, &blueChannel);

		colors[i] = ChannelRGB(fColorR, fColorG, fColorB, redChannel, greenChannel, blueChannel).getColorForLED();
	}

	Handler::getLEDStrip()->write(colors, Handler::getNumLEDs());

	msSinceWaveStart += millis() - prevUpdate;
	prevUpdate = millis();
	delete[] colors;

}

void TwoColorWaveMode::activate() {
	ArduinoMode::activate();
	doneReadingData = false;
}