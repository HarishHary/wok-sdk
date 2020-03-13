#pragma once
#undef CreateFont

struct Vertex_t {
	Vector2D	mPosition;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0)) {
		mPosition = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0)) {
		mPosition = pos;
		m_TexCoord = coord;
	}
};

enum FontFlags_t {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

typedef unsigned long HFont;

class ISurface {
public:
	VFUNC(DrawSetColor(Color color), 14, void(__thiscall*)(void*, Color), color)
	VFUNC(DrawSetColor(int r, int g, int b, int a), 15, void(__thiscall*)(void*, int, int, int, int), r, g, b, a)
	VFUNC(DrawFilledRect(int x0, int y0, int x1, int y1), 16, void(__thiscall*)(void*, int, int, int, int), x0, y0, x1, y1)
	VFUNC(DrawOutlinedRect(int x0, int y0, int x1, int y1), 18, void(__thiscall*)(void*, int, int, int, int), x0, y0, x1, y1)
	VFUNC(DrawLine(int x0, int y0, int x1, int y1), 19, void(__thiscall*)(void*, int, int, int, int), x0, y0, x1, y1)
	VFUNC(DrawSetTextFont(HFont font), 23, void(__thiscall*)(void*, HFont), font)
	VFUNC(DrawSetTextColor(Color color), 24, void(__thiscall*)(void*, Color), color)
	VFUNC(DrawSetTextColor(int r, int g, int b, int a), 25, void(__thiscall*)(void*, int, int, int, int), r, g, b, a)
	VFUNC(DrawSetTextPos(int x, int y), 26, void(__thiscall*)(void*, int, int), x, y)
	VFUNC(DrawPrintText(const wchar_t* text, int text_length), 28, void(__thiscall*)(void*, const wchar_t*, int, int), text, text_length, 0)
	VFUNC(DrawSetTextureRGBA(int id, unsigned char const* colors, int w, int h), 37, void(__thiscall*)(void*, int, unsigned char const*, int, int), id, colors, w, h)
	VFUNC(DrawSetTexture(int id), 38, void(__thiscall*)(void*, int), id)
	VFUNC(IsTextureIDValid(int id), 42, bool(__thiscall*)(void*, int), id)
	VFUNC(CreateNewTextureID(bool procedural = false), 43, int(__thiscall*)(void*, bool), procedural)
	VFUNC(GetScreenSize(int& wide, int& tall), 44, void(__thiscall*)(void*, int&, int&), wide, tall)
	VFUNC(UnlockCursor(), 66, void(__thiscall*)(void*))
	VFUNC(LockCursor(), 67, void(__thiscall*)(void*))
	VFUNC(CreateFont(), 71, HFont(__thiscall*)(void*))
	VFUNC(SetFontGlyphSet(HFont font, const char* font_name, int tall, int weight, int blur, int scan_lines, int flags, int min_range, int max_range), 72, void(__thiscall*)(void*, HFont, const char*, int, int, int, int, int, int, int), font, font_name, tall, weight, blur, scan_lines, flags, min_range, max_range)
	VFUNC(AddCustomFontFile(const char* font_file_name), 73, bool(__thiscall*)(void*, const char*), font_file_name)
	VFUNC(GetFontTall(HFont font), 74, int(__thiscall*)(void*, HFont), font)
	VFUNC(GetFontTall(HFont font, int ch), 78, int(__thiscall*)(void*, HFont, int), font, ch)
	VFUNC(GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall), 79, void(__thiscall*)(void*, unsigned long, const wchar_t*, int&, int&), font, text, wide, tall)
	VFUNC(DrawOutlinedCircle(int x, int y, int radius, int segments), 103, void(__thiscall*)(void*, int, int, int, int), x, y, radius, segments)
	VFUNC(OnScreenSizeChange(int old_width, int old_height), 116, int(__thiscall*)(void*, int, int), old_width, old_height)
	VFUNC(DrawFilledRectFade(int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool horizontal), 123, void(__thiscall*)(void*, int, int, int, int, unsigned int, unsigned int, bool), x0, y0, x1, y1, alpha0, alpha1, horizontal)
	VFUNC(AddBitmapFontFile(const char* font_file_name), 127, bool(__thiscall*)(void*, const char*), font_file_name)
	VFUNC(GetClipRect(int& x0, int& y0, int& x1, int& y1), 146, void(__thiscall*)(void*, int, int, int, int), x0, y0, x1, y1)
	VFUNC(SetClipRect(int& x0, int& y0, int& x1, int& y1), 147, void(__thiscall*)(void*, int, int, int, int), x0, y0, x1, y1)
	VFUNC(DrawTexturedPolygon(int count, Vertex_t* vertex, bool clip = true), 106, void(__thiscall*)(void*, int, Vertex_t*, bool), count, vertex, clip)
	CUSTOM_VFUNC(StartDrawing(), "vguimatsurface.dll", "55 8B EC 83 E4 C0 83 EC 38", void(__thiscall*)(void*))
	CUSTOM_VFUNC(FinishDrawing(), "vguimatsurface.dll", "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?", void(__thiscall*)(void*))
	CUSTOM_VFUNC(DrawColoredText(HFont font, int x, int y, int r, int g, int b, int a, const char* txt), "vguimatsurface.dll", "55 8B EC 81 EC ? ? ? ? 56 57 FF 75 10", void(__thiscall*)(void*, HFont, int, int, int, int, int, int, const char*, const char*), font, x, y, r, g, b, a, txt, nullptr)
};
extern ISurface* g_pSurface;