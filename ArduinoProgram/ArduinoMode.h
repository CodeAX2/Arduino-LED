#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <PololuLedStrip.h>
#include "ChannelRGB.h"

class Handler;

class ArduinoMode {

public:

	enum ModeID : int {
		NO_MODE = -1,
		SETUP_MODE = 0,
		ALL_STATIC_COLOR_MODE = 1,
		GRADIENT_COLOR_MODE = 2,
		INDIVIDUAL_STATIC_COLOR_MODE = 3,
		AUDIO_MODE = 4,
		STARTUP_DATA_SET_MODE = 5,
		TWO_COLOR_WAVE_MODE = 6,
		RAINBOW_COLOR_WAVE_MODE = 7
	};

protected:
	ModeID id;
	int currentByteInStream = 0;
	unsigned long prevUpdate = 0;

public:

	ArduinoMode(ModeID id);

	virtual void handleData(byte data);
	virtual void loop();
	virtual void activate();
	virtual void deactivate();

	ModeID getID();

};

