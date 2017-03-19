#include "Module.h"

Module::Module(byte guardPin, byte buttonPin, byte statusPin, byte warningPin) :
	guardPin(guardPin),
	buttonPin(buttonPin),
	statusPin(statusPin),
	warningPin(warningPin),
	data(),
	lastWarningLight(0),
	lastButtonClick(0),
	lastButtonUp(0),
	clickTime(0) {}