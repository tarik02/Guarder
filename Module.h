#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>

class Module {
public:
	enum class ModuleStatus { Off, On, Warning, WithoutWarning, Delayed, Delayed2 };

	const byte guardPin;
	const byte buttonPin;
	const byte statusPin;
	const byte warningPin;


	ModuleStatus status;
	bool wasOpened;
	
	unsigned long lastWarningLight;
	bool warningLight;

	unsigned long lastButtonClick;
	unsigned long lastButtonUp;
	unsigned long clickTime;


	Module(byte guardPin, byte buttonPin, byte statusPin, byte warningPin);
};

#endif