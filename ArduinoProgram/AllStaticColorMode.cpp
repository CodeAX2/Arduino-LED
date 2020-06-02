#include "AllStaticColorMode.h"
#include "Handler.h"

AllStaticColorMode::AllStaticColorMode() : ArduinoMode(ArduinoMode::ModeID::ALL_STATIC_COLOR_MODE) {}

void AllStaticColorMode::handleData(byte data) {

	switch (currentByteInStream) {
	case 0:
		staticRed = data;
		break;
	case 1:
		staticGreen = data;
		break;
	case 2:
		staticBlue = data;

		int redChannel, greenChannel, blueChannel;
		Handler::getRGBChannels(&redChannel, &greenChannel, &blueChannel);

		ChannelRGB color(staticRed, staticGreen, staticBlue, redChannel,
			greenChannel, blueChannel);

		rgb_color* colors = new rgb_color[Handler::getNumLEDs()];

		for (uint16_t i = 0; i < Handler::getNumLEDs(); i++) {
			colors[i] = color.getColorForLED();
		}

		Handler::getLEDStrip()->write(colors, Handler::getNumLEDs());
		delete[] colors;

		Handler::setCurrentMode(Handler::getModeByID(ArduinoMode::ModeID::NO_MODE));
		break;
	}

	currentByteInStream++;
}