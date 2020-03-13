#pragma once

class IMemAlloc {
public:
	VFUNC(Alloc(size_t size), 1, void*(__thiscall*)(void*, size_t), size)
	VFUNC(Realloc(void* mem, size_t size), 3, void*(__thiscall*)(void*, void*, size_t), mem, size)
	VFUNC(Free(void* mem), 5, void(__thiscall*)(void*, void*), mem)
};
extern IMemAlloc* g_pMemAlloc;