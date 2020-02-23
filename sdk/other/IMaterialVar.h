#pragma once

class IMaterialVar {
public:
	VFUNC(GetTextureValue(), 1, ITexture*(__thiscall*)(void*));
	VFUNC(SetFloatValue(float val), 4, void(__thiscall*)(void*, float), val);
	VFUNC(SetIntValue(int val), 5, void(__thiscall*)(void*, int), val);
	VFUNC(SetStringValue(char const *val), 6, void(__thiscall*)(void*, char const *), val);
	VFUNC(SetVectorValue(float x, float y, float z), 11, void(__thiscall*)(void*, float, float, float), x, y, z);
	VFUNC(SetTextureValue(ITexture* tex), 15, void(__thiscall*)(void*, ITexture*), tex);
};