#include "LEDModeHandler.h"

std::vector<cta::LEDMode*> cta::LEDModeHandler::allModes;

void cta::LEDModeHandler::addMode(cta::LEDMode* mode) {
	allModes.push_back(mode);
}

cta::LEDMode* cta::LEDModeHandler::getModeByType(std::string type) {
	for (int i = 0; i < allModes.size(); i++) {
		if (allModes[i]->getType() == type) return allModes[i];
	}
	return nullptr;
}

int cta::LEDModeHandler::getNumberOfModes() {
	return allModes.size();
}

std::vector<cta::LEDMode*> cta::LEDModeHandler::getAllModes() {
	return allModes;
}