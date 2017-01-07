#include "Guarder.h"
#include "Module.h"



const byte GUARDER_BUZZER_PIN = 12;


const byte MODULE1_GUARD_PIN = 0;
const byte MODULE1_BUTTON_PIN = A5;
const byte MODULE1_STATUS_PIN = 2;
const byte MODULE1_WARNING_PIN = 3;

const byte MODULE2_GUARD_PIN = 1;
const byte MODULE2_BUTTON_PIN = A4;
const byte MODULE2_STATUS_PIN = 4;
const byte MODULE2_WARNING_PIN = 5;

const byte MODULE3_GUARD_PIN = 2;
const byte MODULE3_BUTTON_PIN = 6;
const byte MODULE3_STATUS_PIN = 7;
const byte MODULE3_WARNING_PIN = 8;

const byte MODULE4_GUARD_PIN = 3;
const byte MODULE4_BUTTON_PIN = 9;
const byte MODULE4_STATUS_PIN = 10;
const byte MODULE4_WARNING_PIN = 11;



Guarder guarder(GUARDER_BUZZER_PIN, 4, new Module[4] {
	Module(
		MODULE1_GUARD_PIN,
		MODULE1_BUTTON_PIN,
		MODULE1_STATUS_PIN,
		MODULE1_WARNING_PIN),

	Module(
		MODULE2_GUARD_PIN,
		MODULE2_BUTTON_PIN,
		MODULE2_STATUS_PIN,
		MODULE2_WARNING_PIN),

	Module(
		MODULE3_GUARD_PIN,
		MODULE3_BUTTON_PIN,
		MODULE3_STATUS_PIN,
		MODULE3_WARNING_PIN),

	Module(
		MODULE4_GUARD_PIN,
		MODULE4_BUTTON_PIN,
		MODULE4_STATUS_PIN,
		MODULE4_WARNING_PIN)
});

void setup() {
	guarder.setup();
}

void loop() {
	guarder.loop();
}