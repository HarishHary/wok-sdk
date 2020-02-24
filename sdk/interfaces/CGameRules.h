#pragma once

class CGameRules {
public:
	NETVAR(m_bWarmupPeriod(), bool, _("DT_CSGameRulesProxy"), _("cs_gamerules_data"), _("m_bWarmupPeriod"));
	NETVAR(m_bFreezePeriod(), bool, _("DT_CSGameRulesProxy"), _("cs_gamerules_data"), _("m_bFreezePeriod"));
	NETVAR(m_bIsValveDS(), bool, _("DT_CSGameRulesProxy"), _("cs_gamerules_data"), _("m_bIsValveDS"));
};
extern CGameRules* g_pGameRules;
