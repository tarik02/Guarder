#ifndef MODULEDATA_H
#define MODULEDATA_H

/**
 * ModuleData is class that stores saveable module data.
 */
struct ModuleData {
	enum class ModuleStatus { Idle = 0, Warning, WithoutWarning, Delayed, Delayed2 };

	bool on : 1;
	bool wasOpened : 1;
	bool wasClosed : 1;
	bool warningLight : 1;
	ModuleStatus status;

	inline ModuleData() :
		on(false),
		wasOpened(false),
		wasClosed(false),
		warningLight(false),
		status(ModuleStatus::Idle) {}
};

#endif