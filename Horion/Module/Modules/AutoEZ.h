#pragma once
#include <ctime>

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class AutoEZ : public IModule {
private:
	bool sayUserDied = true;
	bool sayEnemDied = true;

	std::string deathMsg[6] = {
		"stfu fanNN kuku hurensohn Ks omk dont say ez no life",
		"Stupidity kills. In this case, it is you that killed me",
		"Ewww stop using hakcs nab!!1!!",
		"kill me again to die irl",
		"yur hacking stfu nab"
		"Kys nn i have 9000ms"
	};

	std::string killMsg[12] = {
		"shittyy client noob",
		"Hdf du nn ur 'skils' make me laf kid HHHHHH $$$ get fked!",
		"You just lost to a vanilla",
		"LOL! He died in the most humiliating way possible! ",
		"You smell like you farted :yuck:",
		"kys yourself just like i killed you here",
		"keep dying nn",
		"cope nn",
		"seethe and mald nn",
		"nice try nn",
		"hdf nn",
		"so ez 1"
	};

public:
	int kc = 0;
	int dc = 0;
	bool foundPlayer = false;
	bool isDead = false;
	bool antiSpamCheck = true;

	AutoEZ();
	~AutoEZ();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};