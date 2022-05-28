#include "CityESP.h"

#include "../../../Utils/Target.h"

CityESP::CityESP() : IModule(0, Category::VISUAL, "Reminds you to city that guy you're fighting") {
	registerFloatSetting("Range", &this->range, this->range, 0.f, 10.f);
	registerBoolSetting("Incl. Self", &this->inclSelf, this->inclSelf);

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

	g_Data.forEachEntity(weLookForAGuy);
	if (inclSelf)
		guyzz.push_back(g_Data.getLocalPlayer());

	if (!g_Data.canUseMoveKeys() || guyzz.empty() || !g_Data.isInGame() || guyzz[0] == NULL || guyzz.size() == 0)
		return;

	for (auto& guy : guyzz) {

		vec3_ti loc = guy->getHumanPos().floor();
		bool guyExposed = false;

		const vec3_ti sides[4] = {
			loc.add(1, 0, 0),
			loc.sub(1, 0, 0),
			loc.sub(0, 0, 1),
			loc.add(0, 0, 1)
		};

		std::vector<vec3_ti> possibleCities;

		for (vec3_ti side : sides) {
			// check if for empty sides
			C_Block* block = g_Data.getLocalPlayer()->region->getBlock(side);
			C_BlockLegacy* blockLegacy = (block->blockLegacy);
			if (blockLegacy->material->isReplaceable) {
				guyExposed = true;
				break;
			}

			// check for non-bedrock sides
			if (!guyExposed && blockLegacy->blockId != 7) {
				possibleCities.push_back(side);
			} 
		}
	
		if (guyExposed) { // 4-block exposure test failed, now test if hitbox is blocking thingz
			bool actuallyExposed = true;

			const vec3_ti burgerKingBurger[8] = {
				loc.add(1, 0, 0),
				loc.sub(1, 0, 0),
				loc.sub(0, 0, 1),
				loc.add(0, 0, 1),
				loc.add(1, 0, 1),
				loc.sub(1, 0, 1),
				loc.sub(-1, 0, 1),
				loc.add(-1, 0, 1)
			};

			std::vector<vec3_ti> overlapHere;

			for (vec3_ti cornz : burgerKingBurger) {
				if (checkCornerHitboxCollision(&cornz.toVec3t(), guy)) {
					actuallyExposed = false; // dude's corner hitboxes actually don't overlap cornz
				} else {
					overlapHere.push_back(cornz);
				}
			}

			if (!overlapHere.empty()) { // there are overlaps

				std::vector<vec3_ti> toCheck;

				if (overlapHere.size() == 1) {
					/* when a person only overlaps 1 block in addt to his current location:
				*	the overlap is an I shape;
				*		C			 CC		 C
				*	   COC	 CC		CPOC 	CPC			+x
				*	   CPC  COPC	 CC		COC		-z		+z
				*		C	 CC				 C			-x
				*/

					if (overlapHere[0].x > loc.x) {
						toCheck.push_back(loc.add(vec3_ti(2, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(0, 0, -1)));
						toCheck.push_back(loc.add(vec3_ti(0, 0, 1)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, -1)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, 1)));
					}
					else if (overlapHere[0].x < loc.x) {
						toCheck.push_back(loc.add(vec3_ti(-2, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(0, 0, -1)));
						toCheck.push_back(loc.add(vec3_ti(0, 0, 1)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, -1)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, 1)));
					} 
					else if (overlapHere[0].z < loc.z) {
						toCheck.push_back(loc.add(vec3_ti(0, 0, 2)));
						toCheck.push_back(loc.add(vec3_ti(0, 0, -1)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, 1)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, 1)));
					} 
					else {
						toCheck.push_back(loc.add(vec3_ti(0, 0, -2)));
						toCheck.push_back(loc.add(vec3_ti(0, 0, 1)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, 0)));
						toCheck.push_back(loc.add(vec3_ti(-1, 0, -1)));
						toCheck.push_back(loc.add(vec3_ti(1, 0, -1)));
					}
					
				}
				else if (overlapHere.size() == 3) {
					/* When overlap is 3 however, you are checking a square instead
					*	 CC		 CC	    CC     CC
					*	CPOC	COOC   COPC	  COOC  	+x
					*	COOC	CPOC   COOC	  COPC	-z		+z
					*	 CC		 CC	    CC	   CC		-x
					* 
					*	from left to right
					*	1) P has highest X, lowest Z
					*	2) P has lowest X, lowest Z
					*	3) P has highest X, highest Z
					*	4) P has lowest X, highest Z
					*/

					overlapHere.push_back(loc);
					std::vector<vec3_ti*> checkSides;

					for (vec3_ti i : overlapHere) {
						checkSides.push_back(&i.add(1, 0, 0));
						checkSides.push_back(&i.sub(1, 0, 0));
						checkSides.push_back(&i.add(0, 0, 1));
						checkSides.push_back(&i.sub(0, 0, 1));
					}

					// remove duplicates from checkSides
					std::sort(checkSides.begin(), checkSides.end());
					checkSides.erase(std::unique(checkSides.begin(), checkSides.end()), checkSides.end());
					
					for (vec3_ti* i : checkSides) {
						toCheck.push_back(*i);
					}
				} 
				else {
					actuallyExposed = true;
				}

				if (!actuallyExposed) {
					for (vec3_ti i : toCheck) {
						// check if for empty sides
						C_Block* block = g_Data.getLocalPlayer()->region->getBlock(i);
						C_BlockLegacy* blockLegacy = (block->blockLegacy);
						if (blockLegacy->material->isReplaceable) {
							actuallyExposed = true;
							break;
						}

						// check for non-bedrock sides
						if (!actuallyExposed && blockLegacy->blockId != 7) {
							possibleCities.push_back(i);
						} 
					}
				}
			}

			if (actuallyExposed)
				exposee.push_back(guy->getHumanPos());

		} else {
			for (vec3_ti i : possibleCities) {
				highlightCity.push_back(i);
			}
		}
		possibleCities.clear();
	}
}

void CityESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (!g_Data.canUseMoveKeys())
		return;

	if (showExposed) {
		for (vec3_t loc : exposee) {
			//clientMessageF("%f %f %f %i", loc.x, loc.y, loc.z, exposee.size());
			float r = (expR / 255.f);
			float g = (expG / 255.f);
			float b = (expB / 255.f);

			DrawUtils::setColor(r, g, b, 1.f);
			DrawUtils::drawBox(loc.add(-0.5f,0,-0.5f), loc.add(0.5f,1,0.5f), expT, false);
		}
	}

	for (vec3_ti blk : highlightCity) {
		float r = (cityR / 255.f);
		float g = (cityG / 255.f);
		float b = (cityB / 255.f);

		DrawUtils::setColor(r, g, b, 1.f);
		DrawUtils::drawBox(blk.toVec3t(), blk.add(1).toVec3t(), cityT, false);
	}
}
