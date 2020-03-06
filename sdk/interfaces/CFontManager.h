#pragma once

class CFontManager {
public:
	CUSTOM_VFUNC(CreateFont(int unk), "vguimatsurface.dll", "55 8B EC 56 8B F1 57 8B 4E 0C 8B 56 04 8D 41 01 3B C2 7E 0B 2B CA 41 51 8B CE E8 ? ? ? ? FF 46 0C 8B 56 0C 8B 7D 08 2B D7 8B 0E 4A 89 4E 10 85 D2 7E 26 8D 04 FD ? ? ? ? 2B C7 8D 0C 81 8D 04 D5 ? ? ? ? 2B C2 C1 E0 02 50 51 8D 41 1C 50 E8 ? ? ? ? 83 C4 0C 8B 0E 8D 14 FD ? ? ? ? 2B D7", HFont(__thiscall*)(void*, int), unk);
	CUSTOM_VFUNC(GetTextSize(HFont font, const wchar_t* text, int& w, int& h), "vguimatsurface.dll", "55 8B EC 83 EC 18 83 7D 0C 00", HFont(__thiscall*)(void*, HFont, const wchar_t*, int&, int&), font, text, w, h);
	CUSTOM_VFUNC(SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0), "vguimatsurface.dll", "55 8B EC 83 EC 08 8B 45 08 53", void(__thiscall*)(void*, HFont, const char*, int, int, int, int, int, signed int, signed int), font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
};
extern CFontManager* g_pFontManager;