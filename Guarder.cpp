#include "Guarder.h"
#include <EEPROM.h>

Guarder::Guarder(byte buzzerPin, byte modulesCount, Module modules[], unsigned long eepromStart) :
	buzzerPin(buzzerPin),
	modulesCount(modulesCount),
	modules(modules),
	eepromStart(eepromStart),
	lastUpdate(0) {

}

void Guarder::setup() {
	pinMode(buzzerPin, OUTPUT);

	for (byte i = 0; i < modulesCount; ++i) {
		auto module = modules[i];

		pinMode(module.guardPin, INPUT);
		pinMode(module.buttonPin, INPUT);
		pinMode(module.statusPin, OUTPUT);
		pinMode(module.warningPin, OUTPUT);
	}

	{ // Load state from EEPROM
		unsigned long address = 0;
		unsigned long sum = 0x34;
		for (byte i = 0; i < modulesCount; ++i) {
			auto &module = modules[i];
			EEPROM.get(address, module.data);
			sum += module.data.on * 4 + module.data.wasOpened * 3 + module.data.wasClosed * 8 + module.data.warningLight * 2 + (unsigned long)module.data.status;
			address += sizeof(ModuleData);

			module.data.wasOpened = false;
			module.data.wasClosed = false;
		}

		unsigned long esum;
		EEPROM.get(address, esum);
		if (sum != esum) {
			for (byte i = 0; i < modulesCount; ++i) {
				auto &module = modules[i];
				memset(&module.data, 0, sizeof(ModuleData));
			}

			update = true; // Save normal data to EEPROM
		}
	}
}

void Guarder::loop() {
	unsigned long time = millis();
	bool buzzing = false;

	update = false;

	bool updateNow = false;

	if ((time - lastUpdate) > 100) {
		lastUpdate = time;
		updateNow = true;
	}

	for (byte i = 0; i < modulesCount; ++i) {
		auto &module = modules[i];

		auto buttonStatus = digitalRead(module.buttonPin) == HIGH;

		if (buttonStatus) {
			if (module.lastButtonClick == 0) {
				module.lastButtonClick = time;
			}
		} else {
			if (module.lastButtonClick != 0) {
				unsigned long clickTime = time - module.lastButtonClick;
				module.lastButtonClick = 0;

				if (clickTime > 50) {
					if (module.lastButtonUp == 0) {
						module.clickTime = clickTime;
						module.lastButtonUp = time;
					} else {
						onButtonClick(module, 0, true);
						module.lastButtonUp = 0;
					}
				}
			}

			if ((module.lastButtonUp != 0) && (time - module.lastButtonUp > 200)) {
				onButtonClick(module, module.clickTime, false);

				module.lastButtonUp = 0;
			}
		}

		if (updateNow) {
			bool moduleStatus = digitalRead(module.guardPin) == HIGH;

			if (!moduleStatus) {
				module.data.wasClosed = true;
			}

			if ((module.data.status == ModuleData::ModuleStatus::Delayed) ||
				(module.data.status == ModuleData::ModuleStatus::Delayed2)) {
				if (module.lastWarningLight == 0) {
					module.lastWarningLight = time;

					digitalWrite(module.statusPin, LOW);
				} else if (time - module.lastWarningLight > 500) {
					module.data.warningLight = !module.data.warningLight;
					module.lastWarningLight = time;

					digitalWrite(module.statusPin, (module.data.warningLight) ? (HIGH) : (LOW));
				}

				if ((moduleStatus) && (module.data.status == ModuleData::ModuleStatus::Delayed2)) {
					module.data.status = ModuleData::ModuleStatus::Idle;
				} else if ((!moduleStatus) && (module.data.status == ModuleData::ModuleStatus::Delayed)) {
					module.data.status = ModuleData::ModuleStatus::Delayed2;
				}
			} else {
				digitalWrite(module.statusPin, (module.data.on) ? (HIGH) : (LOW));

				if (moduleStatus) { // Door closed
					module.lastWarningLight = 0;

					if (module.data.status == ModuleData::ModuleStatus::WithoutWarning) {
						module.data.status = ModuleData::ModuleStatus::Idle;
					}

					digitalWrite(module.warningPin, (module.data.wasOpened) ? (HIGH) : (LOW));
				} else { // Door opened
					if (module.data.on) {
						if (module.lastWarningLight == 0) {
							module.lastWarningLight = time;

							digitalWrite(module.warningPin, LOW);
						} else if (time - module.lastWarningLight > 250) {
							module.data.warningLight = !module.data.warningLight;
							module.lastWarningLight = time;

							digitalWrite(module.warningPin, (module.data.warningLight) ? (HIGH) : (LOW));
						}

						if (module.data.status != ModuleData::ModuleStatus::WithoutWarning) {
							module.data.status = ModuleData::ModuleStatus::Warning;
						}

						module.data.wasOpened = true;
					} else if ((module.data.wasClosed) && (module.data.wasOpened)) {
						digitalWrite(module.statusPin, LOW);
						digitalWrite(module.warningPin, HIGH);

						module.data.wasOpened = true;
					}
				}
			}
		}

		if ((module.data.on) && (module.data.status == ModuleData::ModuleStatus::Warning)) {
			buzzing = true;
		}
	}

	if (buzzing) {
		if ((time - lastBuzz) > 50) {
			playBuzzer();

			lastBuzz = time;
		}
	} else {
		buzzPosition = 0;

		noTone(buzzerPin);
	}

	if (update) { // Save state to EEPROM
		unsigned long address = 0;
		unsigned long sum = 0x34;
		for (byte i = 0; i < modulesCount; ++i) {
			auto &module = modules[i];
			EEPROM.put(address, module.data);
			sum += module.data.on * 4 + module.data.wasOpened * 3 + module.data.wasClosed * 8 + module.data.warningLight * 2 + (unsigned long)module.data.status;
			address += sizeof(ModuleData);
		}

		EEPROM.put(address, sum);
	}
}

void Guarder::onButtonClick(Module &module, unsigned long clickTime, bool isDouble) {
	if (isDouble) {
		if (module.data.on) {
			module.data.status = ModuleData::ModuleStatus::Delayed;
		}
	} else {
		if ((clickTime > 50) && (clickTime <= 500)) {
			if (module.data.on) {
				module.data.status = ModuleData::ModuleStatus::WithoutWarning;
				module.data.wasOpened = false;
			}
		} else if ((clickTime > 500) && (clickTime <= 2500)) {
			if (module.data.on) {
				module.data.on = false;

				digitalWrite(module.statusPin, LOW);
				digitalWrite(module.warningPin, LOW);
				module.lastWarningLight = 0;
				module.data.warningLight = false;
				module.data.wasOpened = false;
				module.lastButtonClick = 0;
			} else {
				digitalWrite(module.statusPin, HIGH);

				module.data.on = true;
			}

			update = true;
		}
	}
}

void Guarder::playBuzzer() {
	char notes[] = "c2g2C2C2";
	int length = sizeof(notes) / sizeof(notes[0]) / 2;

	auto note = notes[buzzPosition * 2];
	auto beat = (int)(notes[buzzPosition * 2 + 1] - '0');

	char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
	int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

	for (int i = 0; i < 8; i++) {
		if (names[i] == note) {
			tone(buzzerPin, tones[i], 1000 / beat);
			break;
		}
	}

	buzzPosition = (buzzPosition + 1) % length;
}