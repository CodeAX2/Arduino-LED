#pragma once
#include "ArduinoMode.h"
class AllStaticColorMode :
	public ArduinoMode {

public:
	AllStaticColorMode();

	void handleData(byte data) override;

private:
	int staticRed = 0;
	int staticGreen = 0;
	int staticBlue = 0;


};

