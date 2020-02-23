#pragma once

class C_BasePlayer;
class CMoveData;

class IGameMovement {
public:
	VFUNC(ProcessMovement(C_BasePlayer* player, CMoveData* move), 1, void(__thiscall*)(void*, C_BasePlayer*, CMoveData*), player, move); 
	VFUNC(Reset(), 2, void(__thiscall*)(void*));
	VFUNC(StartTrackPredictionErrors(C_BasePlayer* player), 3, void(__thiscall*)(void*, C_BasePlayer*), player);
	VFUNC(FinishTrackPredictionErrors(C_BasePlayer* player), 4, void(__thiscall*)(void*, C_BasePlayer*), player);
};
extern IGameMovement* g_pMovement;