#pragma once
#include "ArduinoMode.h"
class IndividualStaticColorMode :
	public ArduinoMode {

public:
	IndividualStaticColorMode();

	void handleData(byte data) override;

private:
	int numLedsToUpdate = 0;
	int currentLedID = 0;
	int currentLedRed = 0, currentLedGreen = 0, currentLedBlue = 0;
	rgb_color* individualLedStripData;


};

