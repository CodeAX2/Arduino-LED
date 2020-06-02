#pragma once
#include "ArduinoMode.h"
class AudioMode :
	public ArduinoMode {

public:
	AudioMode();

	void handleData(byte data) override;
	void loop() override;
	void activate() override;

private:
	int curAudioRed = 0;
	int curAudioGreen = 0;
	int curAudioBlue = 0;
	int audioByteStreamBegin = 0;
	unsigned int tempAudioLevel = 0;

	bool doneReadingData = false;

	ChannelRGB audioColors[8];
	int numAudioColors = 0;
	unsigned int audioLevel = 0;
	int audioColorIndex = 0;

	int levelsToSave = 15;
	unsigned int prevLevels[15];

	int ticksSinceLastAudioPeak;
	int prevBrightness = 0;
	int maxBrightnessDelta = 10;
	int prevBrightnessDelta = 0;
	int prevPeakValue = 0;
	int prevMinValue = 1000;


};

