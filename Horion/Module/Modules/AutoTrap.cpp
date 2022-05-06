#include "AutoTrap.h"

#include "../../../Utils/Logger.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

AutoTrap::AutoTrap() : IModule(0x0, Category::COMBAT, "Automatically traps the nearest player") {
	this->registerIntSetting("range", &this->range, this->range, 3, 12);
	registerBoolSetting("onClick", &this->onClick, this->onClick);
}

AutoTrap::~AutoTrap() {
}

const char* AutoTrap::getModuleName() {
	return ("AutoTrap");
}

static std::vector<C_Entity*> targetList15;

void findEntity15(C_Entity* currentEntity15, bool isRegularEntity) {
	static auto AutoTrapMod = moduleMgr->getModule<AutoTrap>();

	if (currentEntity15 == nullptr)
		return;

	if (currentEntity15 == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity15, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity15->isAlive())
		return;

	if (!Target::isValidTarget(currentEntity15))
		return;

	float dist = (*currentEntity15->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < AutoTrapMod->range) {
		targetList15.push_back(currentEntity15);

		float dist = (*currentEntity15->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < AutoTrapMod->range) {
			targetList15.push_back(currentEntity15);
		}
	}
}

bool AutoTrap::tryAutoTrap(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}

void AutoTrap::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
		return;

	targetList15.clear();
	g_Data.forEachEntity(findEntity15);

	int place = 1;

	if (onClick) {
		if (GameData::isRightClickDown()) {
			place = 0;
		} else {
			place = 1;
		}
	}

	if (!onClick) {
		place = 0;
	}

	if (!targetList15.empty()) {
		//ground level
		vec3_t enemLoc = (targetList15[0]->eyePos0).floor();
		/* GROUND   FEET     HEAD
	       0 0 N	0 0 N    N N N    +x
	       0 0 0    0 0 0    N 0 N  -z   +z
	       0 0 0	0 0 0    0 N N	  -x
		*/
		f1 = enemLoc.add(1, 0, 1);
		g1 = enemLoc.add(1, -1, 1);

		h1 = enemLoc.add(1, 1, 1);
		h2 = enemLoc.add(0, 1, 1);
		h3 = enemLoc.add(-1, 1, 1);
		h4 = enemLoc.add(-1, 1, 0);
		h5 = enemLoc.add(1, 1, 0);
		h6 = enemLoc.add(1, 1, -1);
		h7 = enemLoc.add(0, 1, -1);

		if (place == 0) {
			// L1 to R4 are x + z sides ground level
			if (!tryAutoTrap(g1))
				;
			if (!tryAutoTrap(f1))
				;
			if (!tryAutoTrap(h1))
				;
			if (!tryAutoTrap(h2))
				;
			if (!tryAutoTrap(h3))
				;
			if (!tryAutoTrap(h4))
				;
			if (!tryAutoTrap(h5))
				;
			if (!tryAutoTrap(h6))
				;
			if (!tryAutoTrap(h7))
				;
		}
	}
}
