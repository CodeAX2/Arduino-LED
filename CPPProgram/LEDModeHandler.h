#pragma once
#include "LEDMode.h"
#include <vector>
namespace cta {

	class LEDModeHandler {

	public:
		/*
		
		Add a mode to the overall collection
		of modes;
		
		*/
		static void addMode(LEDMode* mode);

		/*
		
		Get a pointer to a mode based
		on the type given. If the type
		does not match any mode, a
		nullptr is returned
		
		*/
		static LEDMode* getModeByType(std::string type);

		/*
		
		Returns the total number of modes
		stored.
		
		*/
		static int getNumberOfModes();

		/*
		
		Returns a vector containing pointers
		to all stored modes.

		*/
		static std::vector<LEDMode*> getAllModes();

	private:
		static std::vector<LEDMode*> allModes;

	};

}

