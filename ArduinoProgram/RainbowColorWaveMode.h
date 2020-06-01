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
	unsigned long msSinceRainbowWaveStart = 0;

};

