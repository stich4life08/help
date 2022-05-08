#pragma once
#include "Module.h"

class ArrayList : public IModule {
public:
	SettingEnum Modes;
	ArrayList();
	~ArrayList();

	bool bottom = true;
	bool Backgroundrgba = false;
	bool rgba = false;

	virtual const char* getModuleName() override;
};