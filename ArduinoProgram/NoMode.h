#pragma once
#include "ArduinoMode.h"
class NoMode :
	public ArduinoMode {

public:

	NoMode();
	void handleData(byte data) override;

};

