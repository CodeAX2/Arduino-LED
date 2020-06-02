#include "IndividualStaticColorMode.h"
#include "Handler.h"

IndividualStaticColorMode::IndividualStaticColorMode() : ArduinoMode(ArduinoMode::ModeID::INDIVIDUAL_STATIC_COLOR_MODE) {}

void IndividualStaticColorMode::handleData(byte data) {

	switch (currentByteInStream) {
	case 0:
		numLedsToUpdate = data;
		break;

	case 1:
		numLedsToUpdate += 256 * data;
		individualLedStripData = new rgb_color[Handler::getNumLEDs()];
		for (int i = 0; i < Handler::getNumLEDs(); i++) {
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

			int redChannel, greenChannel, blueChannel;
			Handler::getRGBChannels(&redChannel, &greenChannel, &blueChannel);

			ChannelRGB color(currentLedRed, currentLedGreen, currentLedBlue,
				redChannel, greenChannel, blueChannel);

			individualLedStripData[currentLedID] = color.getColorForLED();

			if (currentColorDataIndex == numLedsToUpdate - 1) {
				Handler::getLEDStrip()->write(individualLedStripData, Handler::getNumLEDs());
				delete[] individualLedStripData;
				Handler::setCurrentMode(Handler::getModeByID(ArduinoMode::ModeID::NO_MODE));
			}

			delete[] individualLedStripData;
			break;
		}

		break;
	}

	currentByteInStream++;
}