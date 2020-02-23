#include "globals.h"

C_CSPlayer* get_entity(const int index) { return reinterpret_cast<C_CSPlayer*>(g_pEntityList->GetClientEntity(index)); }
C_BaseCombatWeapon* get_weapon(CBaseHandle handle) { if (!handle.IsValid()) { return nullptr; } return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(handle)); }

namespace g {
	C_LocalPlayer	local;
	CUserCmd*	cmd = nullptr;
}
