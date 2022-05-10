#include "Arraylist.h"
ArrayList::ArrayList() : IModule(0, Category::VISUAL, "ArrayList") {
	// registerEnumSetting("Modes", &Modes, 0);
	// Modes = SettingEnum(this)
	//.addEntry(EnumEntry("Top", 0))
	//.addEntry(EnumEntry("Bottom", 1));

	registerBoolSetting("Bottom", &this->bottom, this->bottom);
	//registerBoolSetting("RGBAback", &this->Backgroundrgba, this->Backgroundrgba);
	registerBoolSetting("RGBA", &this->rgba, this->rgba);
	registerBoolSetting("Cool", &this->Cool, this->Cool);
}

ArrayList::~ArrayList(){};


const char* ArrayList::getModuleName() {
	return ("ArrayList");
}