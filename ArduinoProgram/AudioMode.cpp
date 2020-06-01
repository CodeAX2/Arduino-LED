#include "AudioMode.h"
#include "Handler.h"
#include "Utils.h"

AudioMode::AudioMode() : ArduinoMode(ArduinoMode::ModeID::AUDIO_MODE) {}

void AudioMode::handleData(byte data) {

	switch (currentByteInStream) {
	case 0:
		numAudioColors = data;
		break;
	default:

		if (!doneReadingData) {
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
				int redChannel, greenChannel, blueChannel;
				Handler::getRGBChannels(&redChannel, &greenChannel, &blueChannel);
				audioColors[currentAudioDataIndex] =
					ChannelRGB(curAudioRed, curAudioGreen, curAudioBlue, redChannel,
						greenChannel, blueChannel);

				if (currentAudioDataIndex == numAudioColors - 1) {
					doneReadingData = true;
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
						Handler::setCurrentMode(Handler::getModeByID(ArduinoMode::ModeID::NO_MODE));
					}
				}
			}
		}

		break;
	}

	currentByteInStream++;

}


void AudioMode::loop() {

	if (!doneReadingData) return;

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
		curColor = Utils::setBrightness(curColor, (float)curBrightness / 255);

		// curColor.setRed(curBrightness);
		// curColor.setGreen(curBrightness);
		// curColor.setBlue(curBrightness);

		rgb_color* colors = new rgb_color[Handler::getNumLEDs()];
		for (uint16_t i = 0; i < Handler::getNumLEDs(); i++) {
			colors[i] = curColor.getColorForLED();
		}
		Handler::getLEDStrip()->write(colors, Handler::getNumLEDs());

		prevUpdate = millis();
		prevBrightnessDelta = curBrightness - prevBrightness;
		prevBrightness = curBrightness;
		delete[] colors;
	}


}

void AudioMode::activate() {
	ArduinoMode::activate();
	doneReadingData = false;
}