#include "../../sdk/sdk.h"

c_netvars::c_netvars() {
	for (auto data = g_pClientDll->GetAllClasses(); data; data = data->m_pNext) {
		if (!data->m_pRecvTable)
			continue;

		dump_recursive(data->m_pNetworkName, data->m_pRecvTable, 0);
	}
}

void c_netvars::dump_recursive(const char* base_class, RecvTable* table, uint16_t offset) {
	for (auto i = 0; i < table->m_nProps; i++) {
		auto prop = &table->m_pProps[i];
		if (!prop
			|| isdigit(prop->m_pVarName[0])
			|| !strcmp(prop->m_pVarName, _("baseclass")))
			continue;

		if (prop->m_RecvType == DPT_DataTable
			&& prop->m_pDataTable != nullptr
			&& prop->m_pDataTable->m_pNetTableName[0] == 'D') 
			dump_recursive(base_class, prop->m_pDataTable, offset + prop->m_Offset);
		
		char hash_name[256];
		strcpy_s(hash_name, base_class);
		strcat_s(hash_name, _("->"));
		strcat_s(hash_name, prop->m_pVarName);
		auto hash = fnv1a_rt(static_cast<const char*>(hash_name));
		
		props[hash] = { prop, uint16_t(offset + prop->m_Offset) };
	}
}