#include "Utils.h"

int main() {

	cta::ControllerApp app;
	if (app.claimMutex()) {
		cta::Utils::app = &app;
		app.begin();
	}

	return 0;
}