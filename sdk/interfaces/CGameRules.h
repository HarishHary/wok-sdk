#pragma once

class CGameRules {
public:
	NETVAR(m_bWarmupPeriod(), bool, "CCSGameRulesProxy->m_bWarmupPeriod")
	NETVAR(m_bFreezePeriod(), bool, "CCSGameRulesProxy->m_bFreezePeriod")
	NETVAR(m_bIsValveDS(), bool, "CCSGameRulesProxy->m_bIsValveDS")
	NETVAR(m_bBombPlanted(), bool, "CCSGameRulesProxy->m_bBombPlanted")
};
extern CGameRules** g_pGameRules;
