#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class HoleFiller : public IModule {
private:
	bool obsidian = true;
	bool bedrock = true;
	bool holeSwitch = true;
	int Odelay = 0;
	float delay = 0.14f;

	bool hasStarted = false;

public:
	int origSlot;

	int range = 5;
	HoleFiller();
	~HoleFiller();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable();
	virtual void onDisable();
	virtual void onTick(C_GameMode* gm) override;
};