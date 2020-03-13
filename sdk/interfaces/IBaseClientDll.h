#pragma once

class ClientClass;

class IBaseClientDll {
public:
	VFUNC(GetAllClasses(), 8, ClientClass*(__thiscall*)(void*))
};
extern IBaseClientDll* g_pClientDll;