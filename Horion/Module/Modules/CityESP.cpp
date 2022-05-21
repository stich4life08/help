#include "CityESP.h"

#include "../../../Utils/Target.h"

CityESP::CityESP() : IModule(0, Category::VISUAL, "Makes it easier to find entities around you") {
	registerFloatSetting("Range", &this->range, this->range, 0.f, 10.f);

	registerBoolSetting("Show Exposed", &this->showExposed, this->showExposed);
	registerIntSetting("ExposedBox-R", &this->expR, this->expR, 0, 255);
	registerIntSetting("ExposedBox-G", &this->expG, this->expG, 0, 255);
	registerIntSetting("ExposedBox-B", &this->expB, this->expB, 0, 255);
	registerFloatSetting("ExposedBox-T", &this->expT, this->expT, 0.f, 1.f);

	registerIntSetting("CityBox-R", &this->cityR, this->cityR, 0, 255);
	registerIntSetting("CityBox-G", &this->cityG, this->cityG, 0, 255);
	registerIntSetting("CityBox-B", &this->cityB, this->cityB, 0, 255);
	registerFloatSetting("CityBox-T", &this->cityT, this->cityT, 0.f, 1.f);
}

CityESP::~CityESP() {
}

const char* CityESP::getModuleName() {
	return ("CityESP");
}

static std::vector<C_Entity*> guyzz;
bool weLookForAGuy(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<CityESP>()->range) {
		guyzz.push_back(curEnt);
		return true;
	}
	return false;
}

void CityESP::onTick(C_GameMode* gm) {
	guyzz.clear();
	highlightCity.clear();
	exposee.clear();

	if (g_Data.getLocalPlayer() == nullptr) return;

	if (!g_Data.canUseMoveKeys() || guyzz.empty() || !g_Data.isInGame() || guyzz[0] == NULL || guyzz.size() == 0)
		return;

	for (auto& guy : guyzz) {

		vec3_ti loc = guy->getHumanPos();
		bool guyExposed = false;

		const vec3_ti sides[4] = {
			loc.add(1, 0, 0),
			loc.sub(1, 0, 0),
			loc.sub(0, 0, 1),
			loc.add(0, 0, 1)
		};

		for (vec3_ti side : sides) {
			// check if for empty sides
			C_Block* block = g_Data.getLocalPlayer()->region->getBlock(side);
			C_BlockLegacy* blockLegacy = (block->blockLegacy);
			if (blockLegacy->material->isReplaceable) {
				guyExposed = true;
				break;
			}

			// check for non-bedrock sides
			if (blockLegacy->blockId != 7) {
				highlightCity.push_back(side);
			} 
		}

		if (guyExposed) {
			exposee.push_back(loc.toVec3t());
		}
	}
}

void CityESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (!g_Data.canUseMoveKeys() || guyzz.empty() || !g_Data.isInGame() || guyzz[0] == NULL || guyzz.size() == 0)
		return;

	
}
