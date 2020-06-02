#pragma once
#include "ArduinoMode.h"
class StartupDataSetMode :
	public ArduinoMode {

public:
	StartupDataSetMode();

	void handleData(byte data) override;

private:
	int numStoredBytes = 0;


};

