#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <PololuLedStrip.h>
#include "ArduinoMode.h"

class Handler {

private:
	static ArduinoMode* currentMode;
	static PololuLedStrip<12> ledStrip;
	static int numLed;
	static int redChannel;
	static int greenChannel;
	static int blueChannel;
	static rgb_color offColor;
	static ArduinoMode* allModes[16];

public:
	/*
	
	Returns a pointer to the current arduino mode
	
	*/
	static ArduinoMode* getCurrentMode();


	/*
	
	Returns a pointer to the mode with a matching
	ModeID, or nullptr if none exist.
	
	*/
	static ArduinoMode* getModeByID(ArduinoMode::ModeID id);

	/*
	
	Returns a pointer to the LED strip for writing colors

	*/
	static PololuLedStrip<12>* getLEDStrip();

	/*
	
	Returns the number of LEDs set on the strip

	*/
	static int getNumLEDs();

	/*
	
	Sets rChannel, gChannel, and bChannel equal to
	the red, green, and blue channels
	
	*/
	static void getRGBChannels(int* rChannel, int* gChannel, int* bChannel);

	/*
	
	Returns the off color of the LED
	
	*/
	static rgb_color getOffColor();

	/*
	
	Sets the current mode to a new mode
	
	*/
	static void setCurrentMode(ArduinoMode* newMode);

	/*
	
	Sets the number of LEDs on the strip
	
	*/
	static void setNumLEDs(int amount);

	/*
	
	Sets the red, green, and blue channels
	If the input is -1, the channel is not changed
	
	*/
	static void setRGBChannels(int rChannel, int gChannel, int bChannel);

	/*
	
	Sets the off color of the LED
	
	*/
	static void setOffColor(rgb_color color);

	/*
	
	Adds a new mode to the list of modes, if
	there is not a mode with the same ModeID
	
	*/
	static void addMode(ArduinoMode* mode);

};

