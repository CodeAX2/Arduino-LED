#pragma once
#include "ArduinoMode.h"
class RainbowColorWaveMode :
	public ArduinoMode {

public:
	RainbowColorWaveMode();

	void handleData(byte data) override;
	void loop() override;
	void activate() override;

private:
	bool doneReadingData = false;
	int rainbowWaveLength = 0;
	int rainbowWaveMSDelay = 0;
	int sat = 255;
	int val = 255;
	unsigned long msSinceRainbowWaveStart = 0;

};

