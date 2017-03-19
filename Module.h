#ifndef MODULE_H
#define MODULE_H

#include "ModuleData.h"
#include <Arduino.h>

class Module {
public:
	const byte guardPin;
	const byte buttonPin;
	const byte statusPin;
	const byte warningPin;

	ModuleData data;
	
	unsigned long lastWarningLight;

	unsigned long lastButtonClick;
	unsigned long lastButtonUp;
	unsigned long clickTime;


	Module(byte guardPin, byte buttonPin, byte statusPin, byte warningPin);
};

#endif