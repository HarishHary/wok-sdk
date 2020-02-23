#pragma once

class CGameRules {
public:
	NETVAR(is_warmup_period(), bool, _("DT_CSGameRulesProxy"), _("cs_gamerules_data "), _("m_bWarmupPeriod"));
	NETVAR(is_freeze_period(), bool, _("DT_CSGameRulesProxy"), _("cs_gamerules_data "), _("m_bFreezePeriod"));
	NETVAR(is_valve_ds(), bool, _("DT_CSGameRulesProxy"), _("cs_gamerules_data "), _("m_bIsValveDS"));
};
extern CGameRules* g_pGameRules;