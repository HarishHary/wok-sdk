#pragma once
#include "../../globals.h"

class c_prediction : public c_singleton<c_prediction>{
private:
	float backup_curtime;
	float backup_frametime;

	CMoveData movedata;
	int* prediction_player;
	int* prediction_random_seed;

    int post_think(C_CSPlayer* player) { // 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07 (client_panorama.dll)
		utils::get_vfunc<void(__thiscall*)(void*)>(g_pModelCache, 33)(g_pModelCache);
		if (player->is_alive()) {
			utils::get_vfunc<void(__thiscall*)(void*)>(player, 339)(player);

			if (player->get_flags() & FL_ONGROUND) 
				*reinterpret_cast<uintptr_t*>(uintptr_t(player) + 0xC05) = 0;

			if (*reinterpret_cast<uint32_t*>(uintptr_t(player) + 0xA2F) == -1)
				utils::get_vfunc<void(__thiscall*)(void*, int)>(player, 218)(player, 0);

			utils::get_vfunc<void(__thiscall*)(void*)>(player, 219)(player);

			static const auto post_think_v_physics_fn = reinterpret_cast<bool(__thiscall*)(C_CSPlayer*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));
			post_think_v_physics_fn(player);
		}

		static const auto simulate_player_simulated_entities_fn = reinterpret_cast<bool(__thiscall*)(C_CSPlayer*)>(SIG("client_panorama.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72"));
		simulate_player_simulated_entities_fn(player);

		return utils::get_vfunc<int(__thiscall*)(void*)>(g_pModelCache, 34)(g_pModelCache);
	}
public:
    void run(C_CSPlayer* player, CUserCmd* cmd);
    void end(C_CSPlayer* player, CUserCmd* cmd);
};
#define engine_prediction c_prediction::instance()