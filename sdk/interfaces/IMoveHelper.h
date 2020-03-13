#pragma once

class C_BasePlayer;

class IMoveHelper {
public:
	VFUNC(SetHost(C_BasePlayer* player), 1, void(__thiscall*)(void*, C_BasePlayer*), player)
	VFUNC(ProcessImpacts(), 4, void(__thiscall*)(void*))
};
extern IMoveHelper* g_pMoveHelper;