#pragma once
#include "../common_includes.h"
#include "../singleton.h"
#include "netvars/datamap.h"
#include "netvars/netvars.h"
#include "memory/memory.h"

namespace utils {
	template <typename T> 
	static T get_vfunc(void* base, const uint16_t index) {
		return (*reinterpret_cast<T**>(base))[index];
	}
}

#define VFUNC(func, index, type, ...) auto func { return utils::get_vfunc<type>(this, index)(this, __VA_ARGS__); };

#define CUSTOM_VFUNC(func, name, sig, type, ...) auto func { static const auto addr = SIG(name, sig); return reinterpret_cast<type>(addr)(this, __VA_ARGS__); };
