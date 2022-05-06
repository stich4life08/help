#include "CrystalAura.h"
CrystalAura::CrystalAura() : IModule(0x0, Category::COMBAT, "CrystalAura by SBReality#1948") {
	dmgEnum = SettingEnum(this)
				  .addEntry(EnumEntry("Vanilla", dmg_vanilla))
				  .addEntry(EnumEntry("Java", dmg_java))
				  .addEntry(EnumEntry("2b2e", dmg_2b2e))
				  .addEntry(EnumEntry("2b2eOLD", dmg_2b2elow));

	registerEnumSetting("Server Mode", &dmgEnum, dmg_java);
	registerBoolSetting("AutoPlace", &autoplace, autoplace);
	registerBoolSetting("MultiAura", &multi, multi);
	// registerBoolSetting("SilentSwitch", &switch_silent, switch_silent);
	registerIntSetting("Range", &range, range, 1, 10);

	registerIntSetting("Proximity", &proximity, proximity, 1, 10);
	registerIntSetting("Place-Delay", &delay, delay, 0, 20);
	registerIntSetting("Break-Delay", &delay2, delay2, 0, 20);

	registerBoolSetting("WallHack", &esp, esp);
	registerBoolSetting("VisualTarget", &VisTarget, VisTarget);
	registerBoolSetting("AntiCrystal", &xc, xc);

	// registerBoolSetting("AutoSwitch", &autoswitch, autoswitch);

	registerBoolSetting("Hurttime", &HurtTime, HurtTime);
	registerEnumSetting("Rotate", &RotMode, rot_none);
	RotMode = SettingEnum(this)
				  .addEntry(EnumEntry("None", rot_none))
				  .addEntry(EnumEntry("Silent", rot_silent))
				  .addEntry(EnumEntry("Normal", rot_client))
				  .addEntry(EnumEntry("Smart", rot_smart));

	switchMode = SettingEnum(this)
					 .addEntry(EnumEntry("None", slot_off))
					 .addEntry(EnumEntry("Normal", slot_nornmal))
					 .addEntry(EnumEntry("Better", slot_better))
					 .addEntry(EnumEntry("Silent", slot_silent));
	registerEnumSetting("Switch", &switchMode, slot_better);

	registerBoolSetting("TargetHUD", &info, info);
	registerBoolSetting("Render", &render, render);
	// registerBoolSetting("bool", &bool1, bool1);
}

CrystalAura::~CrystalAura() {
}

int crystalDelay = 0;
int crystalDelay2 = 0;
int crystalDelay3 = 0;
const char* CrystalAura::getModuleName() {
	return ("CrystalAuraSBR");
}

static std::vector<C_Entity*> targetList7;

void findEntityK3R(C_Entity* currentEntity, bool isRegularEntity) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 71)  // crystal
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
		return;
	if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
		return;
	if (currentEntity->getEntityTypeId() == 64)  // item
		return;
	if (currentEntity->getEntityTypeId() == 69)  // xp_orb
		return;
	if (currentEntity->getEntityTypeId() == 80)  // arrow
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < CrystalAuraMod->range) {
		targetList7.push_back(currentEntity);
	}
}

bool checkTargCollision12(vec3_t* block, C_Entity* ent) {
	std::vector<vec3_t*> corners;
	corners.clear();
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>();
	int dmgMode5 = CrystalAuraMod->dmgEnum.GetSelectedEntry().GetValue();

	if (dmgMode5 == CrystalAuraMod->dmg_2b2e) {
		corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
		corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
		corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
		corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
	}
	int n = 0;
	if (!corners.empty())
		for (auto corner : corners) {
			// DrawUtils::drawText(DrawUtils::worldToScreen(*corners[n]), &std::to_string(n + 1), MC_Color(1.f, 1.f, 1.f));
			// DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
			// if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->blockId != 0) {
			// DrawUtils::drawBox(vec3_t(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), g_Data.getLocalPlayer()->region->getBlock(vec3_ti(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->aabb.upper, 2.f);
			// DrawUtils::drawBox(vec3_t(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), vec3_t(floor(corners[n]->x) + 1.f, floor(corners[n]->y - 0.5f) + 1.f, floor(corners[n]->z) + 1.f), 2.f);
			n++;

			if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
				//	DrawUtils::setColor(1.f, 0.f, 0.f, 0.5f);
				//	DrawUtils::drawBox(block->floor(), {floor(block->x) + 1.f, floor(block->y) + 1.f, floor(block->z) + 1.f}, 0.7f, false);
				return true;
				//	}
			}
		}

	return false;
}
bool findCry3() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 637) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

bool SetOldSlot3() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_ItemStack* stack = inv->getItemStack(supplies->selectedHotbarSlot);
	if (stack->item != nullptr) {
		C_MobEquipmentPacket a(id, *stack, supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		return true;
	}

	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}
bool checkSurrounded21(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;

	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();

	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		if (!checkTargCollision12(&blocks->toVec3t(), ent)) {
			return false;
		}
	}
	return true;
}

std::vector<vec3_t*> getGucciPlacement21(C_Entity* ent) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>();
	int dmgMode2 = CrystalAuraMod->dmgEnum.GetSelectedEntry().GetValue();

	std::vector<vec3_t> SurChecker = {
		vec3_t(1, 0, 0),
		vec3_t(0, 0, 1),
		vec3_t(-1, 0, 0),
		vec3_t(0, 0, -1),
	};
	std::vector<vec3_t> SurChecker2 = {
		vec3_t(1, 1, 0),
		vec3_t(0, 1, 1),
		vec3_t(-1, 1, 0),
		vec3_t(0, 1, -1),
	};
	vec3_t entPos = ent->getPos()->floor();
	// for (int y = entPos.y - 2; y <= entPos.y + 0; y++) {

	int x;
	int y;
	int z;

	bool sendBack = false;
	bool gotoca = false;

	for (auto checkThis : SurChecker) {
		for (auto checkThis2 : SurChecker2) {
			auto playerOn = targetList7[0]->getPos();
			auto rndOffPlayerOn = playerOn->floor();
			rndOffPlayerOn.y -= 1.f;

			auto calcSides = rndOffPlayerOn.add(checkThis);
			auto calcSides2 = rndOffPlayerOn.add(checkThis2);

			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(calcSides))->toLegacy()->blockId == 0) {
				sendBack = true;
			}
		}
	}
	for (auto checkThis : SurChecker) {
		for (auto checkThis2 : SurChecker2) {
			auto playerOn = targetList7[0]->getPos();
			auto rndOffPlayerOn = playerOn->floor();
			rndOffPlayerOn.y -= 1.f;

			auto calcSides = rndOffPlayerOn.add(checkThis);
			auto calcSides2 = rndOffPlayerOn.add(checkThis2);

			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(calcSides2))->toLegacy()->blockId == 0 && sendBack) {
				gotoca = true;
			}
		}
	}
	vec3_t pos = ent->getPos()->floor();
	for (float xa = pos.x - moduleMgr->getModule<CrystalAura>()->proximity; !(xa > pos.x + moduleMgr->getModule<CrystalAura>()->proximity); xa++) {
		for (float ya = pos.y - moduleMgr->getModule<CrystalAura>()->proximity; !(ya > pos.y + moduleMgr->getModule<CrystalAura>()->proximity); ya++) {
			for (float za = pos.z - moduleMgr->getModule<CrystalAura>()->proximity; !(za > pos.z + moduleMgr->getModule<CrystalAura>()->proximity); za++) {
				if (!gotoca) {
					for (int n = -64; n < 319; n++) y = n;
				}
				if (!sendBack) {
					x = xa;
					y = entPos.y;
					z = za;
				} else {
					x = entPos.x;
					y = entPos.y -= 1;
					z = entPos.z;
				}
				std::vector<vec3_t*> finalBlocks;
				std::vector<vec3_ti*> blockChecks;
				blockChecks.clear();
				finalBlocks.clear();
				if (blockChecks.empty()) {
					blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
					blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
					blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
					blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
				}

				for (auto blocks : blockChecks) {
					auto blkID = g_Data.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
					auto blkIDL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 1, blocks->z))->toLegacy()->blockId;
					auto blkIDLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 2, blocks->z))->toLegacy()->blockId;
					auto blkIDLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 3, blocks->z))->toLegacy()->blockId;
					auto blkIDLLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 4, blocks->z))->toLegacy()->blockId;
					auto blkIDLLLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, y, blocks->z))->toLegacy()->blockId;

					if (!checkTargCollision12(&blocks->toVec3t(), ent)) {  // very efficient coding here

						if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
							finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
						} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
							finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
						}

						/*if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
					finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
				} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
					finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
				} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && (blkIDLLL == 7 || blkIDLLL == 49)) {
					finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 2, blocks->z));
				} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && & blkIDLLL == 0 && (blkIDLLLL == 7 || blkIDLLLL == 49)) {
					finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 3, blocks->z));
				}*/
					} else {
						for (int x = entPos.x - 2; x <= entPos.x + 2; x++) {
							for (int y = entPos.y - 2; y <= entPos.y + 2; y++) {
								for (int z = entPos.z - 2; z <= entPos.z + 2; z++) {
									auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
									auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

									if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
										finalBlocks.push_back(new vec3_t(x, y, z));
									}
								}
							}
						}
					}
				}

				return finalBlocks;
			}
		}
	}
}
bool hasPlacedA = false;

void CrystalAura::onEnable() {
	hasPlacedA = false;
}
vec3_t espPosLower;
vec3_t espPosUpper;
vec3_t crystalPos;
std::vector<vec3_t*> placeArr;
std::vector<vec3_t*> hitArr; /*
void getCrystal() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isCrystal()) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
*/
void CrystalAura::onTick(C_GameMode* gm) {
	empty3 = targetList7.empty();
	switch (RotMode.selected) {
	case rot_none:
		off = true;
		silent = false;
		normal = false;
		smart = false;
		break;
	case rot_silent:
		off = false;
		silent = true;
		normal = false;
		smart = false;
		break;
	case rot_client:
		off = false;
		silent = false;
		normal = true;
		smart = false;
		break;
	case rot_smart:
		off = false;
		silent = false;
		normal = false;
		smart = true;
		break;
	}
	switch (switchMode.selected) {
	case slot_off:
		switch_silent = false;
		switch_normal = false;
		switch_better = false;
		break;

	case slot_nornmal:
		switch_silent = false;
		switch_normal = true;
		switch_better = false;
		break;
	case slot_better:
		switch_silent = false;
		switch_normal = false;
		switch_better = true;
		break;
	case slot_silent:
		switch_silent = true;
		switch_normal = false;
		switch_better = false;
		break;
	}

	if (g_Data.getLocalPlayer() == nullptr) return;
	// if (isClick && !g_Data.isRightClickDown()) return;
	// if (g_Data.getLocalPlayer()->getSelectedItemId() == 259) return;
	hasPlacedA = false;

	targetList7.clear();

	g_Data.forEachEntity(findEntityK3R);
	hitArr.clear();
	placeArr.clear();
	if (switch_normal) {
		//	getCrystal();
	}
	int editime = 0;
	int slot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	int better = 0;
	if (switch_better) {
		if (!targetList7.empty()) {
			if (editime == 1) slot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
			editime++;
			if (editime >= 2)  //getCrystal();
				better = 0;
		} else {
			better++;
			editime = 0;
			if (better == 1) g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = slot;
		}
	}

	if (autoplace)
		if ((crystalDelay >= delay) && !(targetList7.empty())) {
			crystalDelay = 0;
			if (switch_silent) findCry3();
			std::vector<vec3_t*> gucciPositions;
			if (multi) {
				for (auto& i : targetList7) gucciPositions = getGucciPlacement21(i);
			} else {
				gucciPositions = getGucciPlacement21(targetList7[0]);
			}

			empty = gucciPositions.empty();

			auto supplies = g_Data.getLocalPlayer()->getSupplies();
			auto inv = supplies->inventory;
			OldSlot = supplies->selectedHotbarSlot;
			// 615 = normal id for crystal || 616 = crystal id for nukkit servers
			if (!gucciPositions.empty())
				if ((g_Data.getLocalPlayer()->getSelectedItemId() == 637 || g_Data.getLocalPlayer()->getSelectedItemId() == 616)) {
					placeArr.clear();
					for (auto place : gucciPositions) {
						if (hasPlacedA && !doMultiple) break;
						if (targetList7.empty()) return;
						if (switch_silent) g_Data.getLocalPlayer()->swing();
						for (int n = -64; n < 319; n++) {
							angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(vec3_t(place->x, place->y - 1, place->z));

							if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(place->x, place->y + n, place->z))->toLegacy()->blockId == 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(place->x, place->y + n, place->z))->toLegacy()->blockId == 42)
								gm->buildBlock(&vec3_ti(place->x, place->y + n, place->z), 1, false);

							placeArr.push_back(new vec3_t(place->x, place->y - 1, place->z));
						}

						if (switch_silent) SetOldSlot3();
						// getCrystal();
						// if (switch_silent) supplies->selectedHotbarSlot = OldSlot;
						hasPlacedA = true;
						moduleMgr->getModule<CrystalAura>()->angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(vec3_t(place->x, place->y + 1, place->z));
					}
				}

			gucciPositions.clear();

		} else if (!targetList7.empty()) {
			crystalDelay++;
		}

	// if (crystalDelay2 >= 20) {
	//		hasPlaced = false;
	// }
	crystalDelay2++;
	if (crystalDelay2 >= delay2) {
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			if (targetList7.empty()) return;
			int id = ent->getEntityTypeId();
			int range = moduleMgr->getModule<CrystalAura>()->range;
			if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
				moduleMgr->getModule<CrystalAura>()->empty2 = true;

				//moduleMgr->getModule<CrystalAura>()->angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(vec3_t(ent->getPos()->x, ent->getPos()->y + 0.7f, ent->getPos()->z));
				g_Data.getCGameMode()->attack(ent);
				hasPlacedA = false;

				if (!moduleMgr->getModule<NoSwing>()->isEnabled())
					g_Data.getLocalPlayer()->swingArm();
			} else {
				moduleMgr->getModule<CrystalAura>()->empty2 = false;
			}
		});  //*/
		crystalDelay2 = 0;
	}
}

void CrystalAura::onDisable() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();

	// supplies->selectedHotbarSlot = OldSlot;

	crystalDelay = 0;
	hasPlacedA = false;
}

static float rcolors[4];

void CrystalAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer != nullptr && GameData::canUseMoveKeys() && render) {
		// Rainbow colors
		{
			if (rcolors[3] < 1) {
				rcolors[0] = 0.2f;
				rcolors[1] = 0.2f;
				rcolors[2] = 1.f;
				rcolors[3] = 1;
			}

			Utils::ApplyRainbow(rcolors, 0.0015f);
		}
		if (!placeArr.empty()) {
			for (auto postt : placeArr) {
				DrawUtils::setColor(rcolors[1], rcolors[2], rcolors[3], 0.6f);
				DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.1f, floor(postt->z) + 1.f), 1.f, false);
			}
		}
		if (!hitArr.empty()) {
			for (auto postt : hitArr) {
				DrawUtils::setColor(rcolors[1], rcolors[2], rcolors[3], 0.6f);
				DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 1.f, false);
			}
		}  //*/
	}
}

void CrystalAura::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
		if (!targetList7.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			//vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList7[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}