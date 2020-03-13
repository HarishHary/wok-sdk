#pragma once
#include "memory.h"

class recv_prop_hook {
public:
	explicit recv_prop_hook(RecvProp* prop) { property = prop; }

	RecvVarProxyFn hook_prop(const RecvVarProxyFn proxy_fn) {
		original_proxy_fn = property->m_ProxyFn;
		set_proxy_func(proxy_fn);
		return original_proxy_fn;
	}

	RecvVarProxyFn get_orig() const { return original_proxy_fn; }
	void set_proxy_func(const RecvVarProxyFn proxy_fn) const { property->m_ProxyFn = proxy_fn; }
private:
	RecvProp* property;
	RecvVarProxyFn original_proxy_fn;
};

class c_hook {
public:
	c_hook() : base{ 0 }, old_base{ nullptr }, length{ 0 }, new_base{ nullptr }, new_base_start{ nullptr } {}
	~c_hook() { unhook_all(); }

	explicit c_hook(void* base_) : base{ 0 }, old_base{ nullptr }, length{ 0 }, new_base{ nullptr }, new_base_start{ nullptr } { init(base_); }
	explicit c_hook(uintptr_t base_) : base{ 0 }, old_base{ nullptr }, length{ 0 }, new_base{ nullptr }, new_base_start{ nullptr } { init(base_); }

	bool init(void* base_) { return init(uintptr_t(base_)); }

	bool init(uintptr_t base_) {
		if (!base_)
			return false;

		base = base_;

		old_base = *reinterpret_cast<uintptr_t**>(base_);
		if (!old_base)
			return false;

		length = get_length();
		if (!length)
			return false;

		new_base = std::make_unique<uintptr_t[]>(length + 1);
		if (!new_base)
			return false;

		auto alloc_base = uintptr_t(new_base.get());

		std::memcpy(reinterpret_cast<void*>(alloc_base), static_cast<const void*>(old_base - 1), (length + 1) * sizeof(uintptr_t));

		new_base_start = reinterpret_cast<uintptr_t*>(alloc_base + sizeof(uintptr_t));

		*reinterpret_cast<uintptr_t*>(base_) = uintptr_t(new_base_start);
		return true;
	}

	template<typename T = uintptr_t>
	__forceinline T hook(uint16_t index, void* func) {
		if (!old_base
			|| !new_base_start
			|| index > length
			|| !func)
			return false;

		new_base_start[index] = uintptr_t(func);
		return reinterpret_cast<T>(old_base[index]);
	}

	bool unhook(uint16_t index) {
		if (!old_base 
			|| !new_base_start
			|| index > length)
			return false;

		old_base[index] = new_base_start[index];
		return true;
	}

	bool unhook_all() {
		if (!base
			|| !old_base)
			return false;

		*reinterpret_cast<uintptr_t*>(base) = uintptr_t(old_base);
		return true;
	}
private:
	uintptr_t base;
	uintptr_t* old_base;
	uint16_t length;
	std::unique_ptr<uintptr_t[]> new_base;
	uintptr_t* new_base_start;

	__forceinline bool valid_code_ptr(uintptr_t addr) const {
		if (!addr)
			return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(reinterpret_cast<const void*>(addr), &mbi, sizeof(mbi)))
			return false;

		if (!(mbi.Protect & PAGE_EXECUTE_READWRITE 
			|| mbi.Protect & PAGE_EXECUTE_READ))
			return false;

		return true;
	}

	__forceinline uint16_t get_length() const {
		uint16_t i = 0;
		while (valid_code_ptr(old_base[i]))
			i++;

		return i;
	}
};

class c_bphook {
public:
	c_bphook() = default;
	explicit c_bphook(void* base_) { base = reinterpret_cast<uintptr_t**>(base_); }

	template<typename T, typename fn>
	T apply(uint32_t index, fn func) {
		DWORD old;
		auto old_func = base[index];
		VirtualProtect(&base[index], sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old);
		base[index] = reinterpret_cast<uintptr_t*>(func);
		VirtualProtect(&base[index], sizeof(uintptr_t), old, &old);
		return reinterpret_cast<T>(old_func);
	}
private:
	uintptr_t** base;
};