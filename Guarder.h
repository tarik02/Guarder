#ifndef GUARDER_H
#define GUARDER_H

#include <Arduino.h>
#include "Module.h"

class Guarder {
private:
	const byte buzzerPin;
	const byte modulesCount;
	Module *modules;
	const unsigned long eepromStart;

	unsigned long lastUpdate;
	bool update;

	byte buzzPosition;
	unsigned long lastBuzz;
public:
	Guarder(byte buzzerPin, byte modulesCount, Module modules[], unsigned long eepromStart = 0);

	void setup();
	void loop();

	void onButtonClick(Module &module, unsigned long clickTime);
	void playBuzzer();
};

#endif