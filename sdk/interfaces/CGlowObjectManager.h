#pragma once

class GlowObjectDefinition_t {
public:
	void Set(Color rgba, int style) {
		m_vGlowColor = Vector(rgba.red / 255.f, rgba.green / 255.f, rgba.blue / 255.f);
		m_flAlpha = rgba.alpha / 255.f;
		m_bRenderWhenOccluded = true;
		m_bRenderWhenUnoccluded = false;
		m_nGlowStyle = style;
	}
public:
	GlowObjectDefinition_t() { memset(this, 0, sizeof(*this)); }

	class IClientEntity* m_pEntity;    //0x0000
	union {
		Vector m_vGlowColor;           //0x0004
		struct {
			float   m_flRed;           //0x0004
			float   m_flGreen;         //0x0008
			float   m_flBlue;          //0x000C
		};
	};

	float   m_flAlpha;                 //0x0010
	uint8_t pad_0014[4];               //0x0014
	float   m_flSomeFloat;             //0x0018
	uint8_t pad_001C[4];               //0x001C
	float   m_flAnotherFloat;          //0x0020
	bool    m_bRenderWhenOccluded;     //0x0024
	bool    m_bRenderWhenUnoccluded;   //0x0025
	bool    m_bFullBloomRender;        //0x0026
	uint8_t pad_0027[5];               //0x0027
	int32_t m_nGlowStyle;              //0x002C
	int32_t m_nSplitScreenSlot;        //0x0030
	int32_t m_nNextFreeSlot;           //0x0034

	bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	static const int END_OF_FREE_LIST = -1;
	static const int ENTRY_IN_USE = -2;
}; //Size: 0x0038 (56)

class CGlowObjectManager {
public:
	CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions; //0x0000
	int m_nFirstFreeSlot;                                       //0x000C
};
extern CGlowObjectManager* g_pGlowManager;