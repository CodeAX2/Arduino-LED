#pragma once
#include "ArduinoMode.h"
class TwoColorWaveMode :
	public ArduinoMode {

public:
	TwoColorWaveMode();

	void handleData(byte data) override;
	void loop() override;
	void activate() override;

private:
	bool doneReadingData = false;

	ChannelRGB waveColor1, waveColor2;
	int waveLength = 0;
	int waveMSDelay = 0;
	unsigned long msSinceWaveStart = 0;



};

