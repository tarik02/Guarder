#include "Module.h"

Module::Module(byte guardPin, byte buttonPin, byte statusPin, byte warningPin) :
	guardPin(guardPin),
	buttonPin(buttonPin),
	statusPin(statusPin),
	warningPin(warningPin),
	lastWarningLight(0),
	warningLight(false),
	status(ModuleStatus::Off),
	wasOpened(false),
	lastButtonClick(0) {

}