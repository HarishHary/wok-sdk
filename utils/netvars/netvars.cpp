#include "../../sdk/sdk.h"

void c_netvar_tree::init() {
	if (!g_pClientDll)
		return;

	ClientClass* client_class = g_pClientDll->GetAllClasses();
	if (!client_class)
		return;

	while (client_class != nullptr) {
		auto class_info = std::make_shared<node_t>(0);

		auto recv_table = client_class->m_pRecvTable;

		populate_nodes(recv_table, &class_info->nodes);

		nodes.emplace(recv_table->m_pNetTableName, class_info);

		client_class = client_class->m_pNext;
	}
}

void c_netvar_tree::populate_nodes(RecvTable* recv_table, map_type* map) {
	for (int i = 0; i < recv_table->m_nProps; i++) {
		auto prop = &recv_table->m_pProps[i];
		auto prop_info = std::make_shared<node_t>(prop->m_Offset, prop);

		if (prop->m_RecvType == DPT_DataTable)
			populate_nodes(prop->m_pDataTable, &prop_info->nodes);

		map->emplace(prop->m_pVarName, prop_info);;
	}
}

std::unique_ptr<c_netvar_tree> netvar_tree;