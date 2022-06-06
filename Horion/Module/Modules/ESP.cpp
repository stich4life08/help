#include "ESP.h"

#include "../../../Utils/Target.h"

ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you") {
	this->registerBoolSetting("Rainbow", &this->doRainbow, this->doRainbow);
	this->registerBoolSetting("MobESP", &this->isMobEsp, this->isMobEsp);
	this->registerBoolSetting("ItemESP", &this->isItemESP, this->isItemESP);
	this->registerBoolSetting("2d", &this->is2d, this->is2d);
	this->registerBoolSetting("spidrs r fkin rar", &this->spiderESP, this->spiderESP);
}

ESP::~ESP() {
}

const char* ESP::getModuleName() {
	return ("ESP");
}

static float rcolors[4];

void doRenderStuff(C_Entity* ent, bool isRegularEntitie) {
	static auto espMod = moduleMgr->getModule<ESP>();

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (ent == NULL)
		return;
	if (ent == nullptr)
		return;
	if (ent == localPlayer)
		return;
	//if (ent->timeSinceDeath > 0)
		//return;
	if (!ent->isAlive())
		return;

	if (ent != localPlayer) {
		static auto noFriendsModule = moduleMgr->getModule<NoFriends>();

		if (espMod->isItemESP) {
			if (ent->getEntityTypeId() == 64) {
				DrawUtils::setColor(.01176f, .654902f, .6392157f, 1.f);
				DrawUtils::draw2D(ent, (float)fmax(0.2f, 1 / (float)fmax(1, (*localPlayer->getPos()).dist(*ent->getPos()))));
			}
		}

		if (ent == localPlayer) return;
		if (ent->timeSinceDeath > 0)
			return;
		if (!noFriendsModule->isEnabled() && FriendList::findPlayer(ent->getNameTag()->getText())) {
			DrawUtils::setColor(0.1f, 0.9f, 0.1f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
		} else if (Target::isValidTarget(ent)) {
			if (espMod->doRainbow)
				DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
			else
				DrawUtils::setColor(0.9f, 0.9f, 0.9f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
		} else if (espMod->isMobEsp) {
			if (ent->getEntityTypeId() == 64)  // item
				return;
			if (ent->getEntityTypeId() == 319)  // player
				return;
			if (ent->isInvisible())
				return;
			if (!localPlayer->canAttack(ent, false))
				return;
			if (espMod->spiderESP)
				if (ent->getEntityTypeId() == 35 || ent->getEntityTypeId() == 40) return;

			DrawUtils::setColor(0.2f, 0.2f, 0.9f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
		} else if (espMod->spiderESP) {
			if (ent->getEntityTypeId() == 35 || ent->getEntityTypeId() == 40)
				DrawUtils::setColor(0.4f, 0.7f, 0.1f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
			else
				return;
		} else
			return;

		if (espMod->is2d)
			DrawUtils::draw2D(ent, (float)fmax(0.4f, 1 / (float)fmax(1, localPlayer->getPos()->dist(*ent->getPos()) * 3.f)));
		else
			DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, localPlayer->getPos()->dist(*ent->getPos()))));
	}
}

void ESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
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

		g_Data.forEachEntity(doRenderStuff);
	}
}
