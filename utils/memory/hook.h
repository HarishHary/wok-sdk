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

	RecvVarProxyFn get_orig() const {
		return original_proxy_fn;
	}

	void set_proxy_func(const RecvVarProxyFn proxy_fn) const {
		property->m_ProxyFn = proxy_fn;
	}
private:
	RecvProp* property = nullptr;
	RecvVarProxyFn original_proxy_fn = nullptr;
};

template<class entity>
class c_hook {
public:
	c_hook() = default;
	c_hook(entity* ent) { setup(ent); }

	void setup(entity* ent) {
		base = reinterpret_cast<uintptr_t*>(ent);
		original = *base;

		const auto l = length() + 1;
		current = std::make_unique<uint32_t[]>(l);
		std::memcpy(current.get(), reinterpret_cast<void*>(original - sizeof(uint32_t)), l * sizeof(uint32_t));
		oldbase = base;
		patch_pointer(base);
	}

	template<typename function, typename original_function>
	function apply(const uint32_t index, original_function func) {
		auto old = reinterpret_cast<uintptr_t*>(original)[index];
		current.get()[index + 1] = reinterpret_cast<uintptr_t>(func);
		return reinterpret_cast<function>(old);
	}

	void unapply(const uint32_t index) {
		current.get()[index + 1] = reinterpret_cast<uintptr_t*>(original)[index];
		DWORD old;
		VirtualProtect(&base, sizeof(uintptr_t), PAGE_READWRITE, &old);
		*base = *oldbase;
		VirtualProtect(&base, sizeof(uintptr_t), old, &old);
	
	}

	void patch_pointer(uintptr_t* location) const {
		if (!location)
			return;

		DWORD old;
		PVOID address = location;
		ULONG size = sizeof(uintptr_t);
		VirtualProtect(&address, sizeof(uintptr_t), PAGE_READWRITE, &old);
		*location = reinterpret_cast<uint32_t>(current.get()) + sizeof(uint32_t);
		VirtualProtect(&address, sizeof(uintptr_t), old, &old);
	}
private:
	uint32_t length() const {
		uint32_t index;
		const auto vmt = reinterpret_cast<uint32_t*>(original);

		for (index = 0; vmt[index]; index++)
			if (IS_INTRESOURCE(vmt[index]))
				break;

		return index;
	}

	std::uintptr_t* base;
	std::uintptr_t* oldbase;
	std::uintptr_t original;

	std::unique_ptr<uint32_t[]> current;
};

class c_bphook {
public:
	c_bphook() = default;
	c_bphook(void* ent) { base = reinterpret_cast<uintptr_t**>(ent); }

	template<typename function, typename original_function>
	function apply(uint32_t index, original_function func) {
		DWORD old;
		auto old_func = base[index];
		VirtualProtect(&base[index], sizeof uintptr_t, PAGE_EXECUTE_READWRITE, &old);
		base[index] = reinterpret_cast<uintptr_t*>(func);
		VirtualProtect(&base[index], sizeof uintptr_t, old, &old);
		return reinterpret_cast<function>(old_func);
	}
private:
	std::uintptr_t** base;
};