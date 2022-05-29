#pragma once

#include "Module.h"
#include "../ModuleManager.h"

class APVPDisabler : public IModule {
private:
	int angle = -90;
	bool onlyOnBuildMods = true;
	bool pauseOnXP = true;
	bool pauseOnBow = true;

public:
	APVPDisabler();
	~APVPDisabler();

	virtual const char* getModuleName() override;
	virtual void onPlayerTick(C_Player* player) override;
};