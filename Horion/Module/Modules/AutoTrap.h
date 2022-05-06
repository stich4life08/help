#pragma once
#include "Module.h"
class AutoTrap : public IModule {
private:
	bool tryAutoTrap(vec3_t AutoTrap);

	/* GROUND   FEET     HEAD
	   0 0 N	0 0 N    N N N
	   0 0 0    0 0 0    N 0 N
	   0 0 0	0 0 0    0 N N
	*/

	vec3_t h1;
	vec3_t h2;
	vec3_t h3;
	vec3_t h4;
	vec3_t h5;
	vec3_t h6;
	vec3_t h7;

	vec3_t g1;
	vec3_t f1;

public:
	AutoTrap();
	~AutoTrap();

	int range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
