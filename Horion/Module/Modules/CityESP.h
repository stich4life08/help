#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class CityESP : public IModule {
private:
	bool showExposed = true;

	int expR = 255;
	int expG = 255;
	int expB = 0;
	float expT = 0.4f;

	int cityR = 0;
	int cityG = 255;
	int cityB = 50;
	float cityT = 0.4f;

public:
	std::vector<vec3_ti> highlightCity;
	std::vector<vec3_t> exposee;

	float range = 10.f;
	CityESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};