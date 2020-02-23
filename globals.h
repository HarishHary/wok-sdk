#pragma once
#include "sdk/sdk.h"

extern C_CSPlayer* get_entity(int index);
extern C_BaseCombatWeapon* get_weapon(CBaseHandle handle);

namespace g {
	extern C_LocalPlayer	local;
	extern CUserCmd*	cmd;
}
