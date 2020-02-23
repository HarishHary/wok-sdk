#pragma once
#include "../../common_includes.h"
#include "../obfuscation/obfuscation.h"

namespace memory {
	template<typename T>
	T* capture_interface(const char* module_name, const char* interface_name) {
		auto module_handle = GetModuleHandleA(module_name);
		if (!module_handle)
			return nullptr;

		typedef void*(*fn)(const char *name, int *code);
		auto create_interface_fn = reinterpret_cast<fn>(GetProcAddress(module_handle, _("CreateInterface")));
		if (!create_interface_fn)
			return nullptr;

		return reinterpret_cast<T*>(create_interface_fn(interface_name, nullptr));
	}

	__forceinline uint32_t get_vtable_length(void* table) {
		uint32_t index;
		auto vtable = reinterpret_cast<uint32_t*>(table);
		for (index = 0; vtable[index]; index++)
			if (IS_INTRESOURCE(vtable[index]))
				break;

		return index;
	}

	__forceinline uintptr_t get_module_handle(const uint32_t module, const uint32_t process = 0) {
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(MODULEENTRY32);
		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);

		if (Module32First(snapshot, &entry)) {
			while (Module32Next(snapshot, &entry)) {
				std::string name = entry.szModule;
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);

				if (fnv1a_rt(name.c_str()) == module) {
					CloseHandle(snapshot);
					return reinterpret_cast<uintptr_t>(entry.hModule);
				}
			}
		}
		CloseHandle(snapshot);
		return 0;
	}

	__forceinline uint8_t* find_sig(const uint32_t offset, const char* signature, const uint32_t range = 0u) {
		static auto pattern_to_bytes = [](const char* pattern) -> std::vector<int> {
			auto bytes = std::vector<int32_t>{ };
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					current++;

					if (*current == '?')
						current++;

					bytes.push_back(-1);
				}
				else
					bytes.push_back(static_cast<int32_t>(strtoul(current, &current, 0x10)));
			}

			return bytes;
		};

		const auto scan_bytes = reinterpret_cast<uint8_t*>(offset);
		auto pattern_bytes = pattern_to_bytes(signature);
		const auto s = pattern_bytes.size();
		const auto d = pattern_bytes.data();

		for (auto i = 0ul; i < range - s; ++i) {
			auto found = true;

			for (auto j = 0ul; j < s; ++j)
				if (scan_bytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}

			if (found)
				return &scan_bytes[i];
		}

		return nullptr;
	}

	__forceinline uint8_t* find_module_sig(const uint32_t name, const char* signature) {
		const auto module = get_module_handle(name);

		if (module) {
			const auto dos_header = PIMAGE_DOS_HEADER(module);
			const auto nt_headers = PIMAGE_NT_HEADERS(reinterpret_cast<uint8_t*>(module) + dos_header->e_lfanew);

			return find_sig(module, signature, nt_headers->OptionalHeader.SizeOfImage);
		}

		return nullptr;
	}
}

#define SIG(name, sig) memory::find_module_sig(fnv1a(name), _ot(sig))