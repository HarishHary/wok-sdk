#include "globals.h"

C_CSPlayer* get_entity(int index) { return reinterpret_cast<C_CSPlayer*>(g_pEntityList->GetClientEntity(index)); }

namespace g {
	C_LocalPlayer	local;
	CUserCmd*	cmd = nullptr;
}
