#pragma once

class RecvProp;
class RecvTable;
struct CRecvProxyData;
using RecvVarProxyFn = void(__cdecl *)(CRecvProxyData*, void*, void*);

struct DVariant {
	union {
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		Vector m_Vector;
		int64_t m_Int64;
	};
	int m_Type;
};

struct CRecvProxyData {
	const RecvProp* m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

enum SendPropType : int {
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

class RecvProp {
public:
	char* m_pVarName;
	SendPropType m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	RecvVarProxyFn m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};

class RecvTable {
public:
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};


class c_netvar_tree {
	struct node_t;

	using map_type = std::unordered_map<std::string, std::shared_ptr<node_t>>;

	struct node_t {
		node_t(int offset, RecvProp* prop = nullptr) : offset(offset), prop(prop) {};

		map_type nodes;

		int offset;
		RecvProp* prop;
	};

	map_type nodes{};
public:
	void init();

	template <typename... T> 
	int get_offset(const char* name, T ... args) {
		auto& node = nodes[name];
		return get_offset_recursive(node->nodes, node->offset, args...);
	}

	RecvProp* get_prop(const char* name, const char* prop_name) {
		auto& node = nodes[name];
		return node->nodes[prop_name]->prop;
	}

private:
	void populate_nodes(RecvTable* recv_table, map_type* map);

	static int get_offset_recursive(map_type& map, int acc, const char* name) {
		return acc + map[name]->offset;
	}

	template <typename... T> 
	int get_offset_recursive(map_type& map, int acc, const char* name, T ... args) {
		auto& node = map[name];
		return get_offset_recursive(node->nodes, acc + node->offset, args...);
	}
};

extern std::unique_ptr<c_netvar_tree> netvar_tree;

#define NETPROP(name, name_, prop_name) \
	static RecvProp* name { \
		static const auto prop = netvar_tree->get_prop(name_, prop_name); \
		return prop; \
	};

#define NETVAR(name, type, ...) \
	type& name { \
		static const auto offset = netvar_tree->get_offset(__VA_ARGS__); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
	};

#define NETVAR_OFFSET(name, type, add, ...) \
	type& name { \
		static const auto offset = netvar_tree->get_offset(__VA_ARGS__); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset + add); \
	};

#define OFFSET(name, type, offset) \
	type& name { \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
	};