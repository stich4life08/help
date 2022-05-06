#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class CrystalAura : public IModule {
private:
public:
	int delay = 0;   // TODO: PlaceDelay
	int delay2 = 0;  // TODO: BreakDelay

	bool autoplace = true;
	bool doMultiple = false;  // TODO: 2B2E BETER CA VERY GOOD PLACER CA
	int minHealth = 7;
	int OldSlot;
	bool switched = false;
	bool HurtTime = false;
	bool VisTarget = false;
	bool esp = false;
	bool xc = false;
	bool multi = true;
	int proximity = 3;
	vec2_t angle;

	SettingEnum RotMode;
	static int constexpr rot_none = 0;  // TODO: Implement ray tracing
	static int constexpr rot_silent = 1;
	static int constexpr rot_client = 2;
	static int constexpr rot_smart = 3;

	SettingEnum switchMode;
	static int constexpr slot_off = 0;  // TODO: Implement ray tracing
	static int constexpr slot_nornmal = 1;
	static int constexpr slot_better = 2;
	static int constexpr slot_silent = 3;

	CrystalAura();
	~CrystalAura();
	//virtual const char* getModeName() {
	//return ("Smart");
	//}
	// switchmode
	bool switch_silent = false;
	bool switch_normal = false;
	bool switch_better = false;
	//

	// rotate list
	bool off = false;
	bool silent = false;
	bool normal = false;
	bool smart = false;
	//

	// emptyList
	bool empty = false;
	bool empty2 = false;
	bool empty3 = false;
	//

	int range = 7;
	int timer5 = 0;
	bool info = false;
	bool render = false;
	std::vector<vec3_t> SurChecker = {
		vec3_t(1, 0, 0),
		vec3_t(0, 0, 1),
		vec3_t(-1, 0, 0),
		vec3_t(0, 0, -1),
		vec3_t(0, -1, 0),
	};

	SettingEnum dmgEnum;
	static int constexpr dmg_vanilla = 0;  // TODO: Implement ray tracing
	static int constexpr dmg_java = 1;
	static int constexpr dmg_2b2e = 2;
	static int constexpr dmg_2b2elow = 3;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual void onLevelRender() override;
	// virtual const char* getModName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet) override;
};
