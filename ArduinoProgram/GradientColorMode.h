#pragma once
#include "ArduinoMode.h"
class GradientColorMode :
	public ArduinoMode {

public:
	GradientColorMode();

	virtual void handleData(byte data);
	virtual void loop();
	virtual void activate();

private:

	int curGradientRed = 0;
	int curGradientGreen = 0;
	int curGradientBlue = 0;
	unsigned int curFadeTime = 0;

	ChannelRGB gradientColors[8];
	unsigned int fadeTimes[8];
	int numGradientColors = 0;

	bool doneReadingData = false;

	int curColorIndex = 0;
	unsigned long curColorFadeTimer = 0;
	int tickCounter = 0;
	int strobeSpeed = 0;

};

