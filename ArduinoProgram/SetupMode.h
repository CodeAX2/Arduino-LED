#pragma once
#include "ArduinoMode.h"
class SetupMode :
	public ArduinoMode {

public:
	SetupMode();

	void handleData(byte data) override;


};

