#pragma once

class IInputSystem {
public:
	VFUNC(EnableInput(bool bEnable), 11, void(__thiscall*)(void*, bool), bEnable)
	VFUNC(ResetInputState(), 39, void(__thiscall*)(void*))
};
extern IInputSystem* g_pInputSystem;