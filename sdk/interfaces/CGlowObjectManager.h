#pragma once

class CGlowObjectManager {
public:
	class GlowObjectDefinition_t {
	public:
		void Set(Color rgba, int style) {
			m_vGlowColor = Vector(rgba.red / 255.f, rgba.green / 255.f, rgba.blue / 255.f);
			m_flAlpha = rgba.alpha / 255.f;
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_nGlowStyle = style;
			m_flBloomAmount = 0.8f;
		}
	public:
		C_BaseEntity* m_pEntity;
		Vector	m_vGlowColor;
		float   m_flAlpha;

		char	pad0[8];
		float	m_flBloomAmount;
		char	pad1[4];
		bool    m_bRenderWhenOccluded;
		bool    m_bRenderWhenUnoccluded;
		bool    m_bFullBloomRender;
		char	pad2[1];
		int		m_nFullBloomStencilTestValue;
		int		m_nGlowStyle;
		int		m_nSplitScreenSlot;
		int		m_nNextFreeSlot;

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};
extern CGlowObjectManager* g_pGlowManager;