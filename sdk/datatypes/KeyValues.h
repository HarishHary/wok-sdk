#pragma once

class KeyValues {
public:
	~KeyValues() { g_pMemAlloc->Free(this); }

	void Init(const char* key_name) {
		static const auto init_key_values_fn = reinterpret_cast<void(__thiscall*)(void*, const char*)>(SIG("client_panorama.dll", "55 8B EC 51 33 C0 C7 45"));
		init_key_values_fn(this, key_name);
	}

	void LoadFromBuffer(const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr) {
		static const auto load_from_buffer_fn = reinterpret_cast<void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"));
		load_from_buffer_fn(this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
	}

	KeyValues* FindKey(const char* name, bool bCreate) {
		static auto find_key_fn = reinterpret_cast<KeyValues*(__thiscall*)(KeyValues*, const char*, bool)>(SIG("client_panorama.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
		return find_key_fn(this, name, bCreate);
	}

	void SetInt(const char* keyName, int val) {
		auto key_int = FindKey(keyName, true);
		if (!key_int)
			return;

		*reinterpret_cast<int*>(uintptr_t(key_int) + 0xC) = val;
		*reinterpret_cast<char*>(uintptr_t(key_int) + 0x10) = 2;
	}

	void SetString(const char* name, const char* a2) {
		auto key = FindKey(name, true);
		if (!key)
			return;

		static auto set_string_fn = reinterpret_cast<void(__thiscall*)(void*, const char*)>(SIG("client_panorama.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
		set_string_fn(key, a2);
	}
};