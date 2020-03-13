#pragma once
#include <map>

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

class c_netvars {
private:
	c_netvars();

	struct data {
		RecvProp* prop;
		uint16_t offset;
	};

	std::unordered_map<uint32_t, data> props;

	void dump_recursive(const char* base_class, RecvTable* table, uint16_t offset);
	unsigned short get_offset(unsigned int hash) { return props[hash].offset; }
public:
	static c_netvars& get() { static c_netvars instance; return instance; }
	RecvProp* get_prop(unsigned int hash) { return props[hash].prop; }
	__declspec(noinline) static uint16_t get_offset_by_hash(uint32_t hash) { return get().get_offset(hash); }
};

#define NETVAR(func, type, name) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = c_netvars::get_offset_by_hash(hash); \
		return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
	}

#define PNETVAR(func, type, name) \
	type* func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = c_netvars::get_offset_by_hash(hash); \
		return reinterpret_cast<type*>(uintptr_t(this) + offset); \
	}

#define NETVAR_OFFSET(func, type, name, add) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = c_netvars::get_offset_by_hash(hash); \
		return *reinterpret_cast<type*>(uintptr_t(this) + offset + add); \
	}

#define NETPROP(func, name) \
	static RecvProp* func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto prop = c_netvars::get().get_prop(hash); \
		return prop; \
	}

#define OFFSET(func, type, offset) \
	type& func { \
		static const auto offset_ = offset; \
		return *reinterpret_cast<type*>(uintptr_t(this) + offset_); \
	}

#define POFFSET(func, type, offset) \
	type* func { \
		static const auto offset_ = offset; \
		return reinterpret_cast<type*>(uintptr_t(this) + offset_); \
	}

#define PPOFFSET(func, type, offset) \
	type& func { \
		static const auto offset_ = offset; \
		return **reinterpret_cast<type**>(uintptr_t(this) + offset_); \
	}