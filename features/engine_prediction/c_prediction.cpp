#include "c_prediction.h"
#include "../features.h"

/* 55 8B EC 83 E4 C0 83 EC 34 53 56 8B 75 08 9 (client_panorama.dll) */

void c_prediction::run(C_CSPlayer* player, CUserCmd* cmd) {
	if (!player 
        || g_pEngine->IsPlayingDemo()
		|| !player->is_alive())
		return;

	!move_data ? move_data = g_pMemAlloc->Alloc(182u) : 0;

	if (!prediction_random_seed || !prediction_player) {
		prediction_random_seed = *reinterpret_cast<int**>(SIG("client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86") + 0x1);
		prediction_player = *reinterpret_cast<int**>(SIG("client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48") + 0x2);
	}

	g_pPrediction->Update(g_pClientState->m_nDeltaTick, g_pClientState->m_nDeltaTick > 0,
		g_pClientState->last_command_ack, g_pClientState->lastoutgoingcommand + g_pClientState->chokedcommands);

	backup_curtime = g_pGlobalVars->curtime;
	backup_frametime = g_pGlobalVars->frametime;

	auto backup_in_prediction = g_pPrediction->m_bInPrediction;
	auto backup_is_first_time_predicted = g_pPrediction->m_bFirstTimePredicted;

	g_pGlobalVars->curtime = TICKS_TO_TIME(player->get_tickbase());
	g_pGlobalVars->frametime = g_pPrediction->m_bEnginePaused ? 0.f : g_pGlobalVars->interval_per_tick;

	g_pPrediction->m_bInPrediction = true;
	g_pPrediction->m_bFirstTimePredicted = false;

	*reinterpret_cast<CUserCmd**>(uintptr_t(player) + 0x3338) = cmd;
	*reinterpret_cast<CUserCmd**>(uintptr_t(player) + 0x3288) = cmd;
	
	*prediction_random_seed = cmd ? MD5_PseudoRandom(cmd->commandnumber) & 0x7FFFFFFF : -1;
	*prediction_player = reinterpret_cast<int>(player);

	auto unk0 = *reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x3334);
	auto unk1 = *reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x3330);

	cmd->buttons |= unk0;
	cmd->buttons &= ~unk1;
	
	g_pMoveHelper->SetHost(player);
	g_pMovement->StartTrackPredictionErrors(player);

	if (cmd->weaponselect != 0) {
		auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(cmd->weaponselect));
		if (weapon) {
			auto weapon_data = weapon->get_cs_weapon_data();
			weapon_data ? player->select_item(weapon_data->szWeaponName, cmd->weaponsubtype) : 0;
		}
	}

	auto vehicle = player->get_vehicle().IsValid() ? reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntityFromHandle(player->get_vehicle())) : nullptr;
	if (cmd->impulse && (!vehicle || player->using_standard_weapons_in_vehicle()))
		*reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x31FC) = cmd->impulse;

	auto buttons = cmd->buttons;
	auto buttons_last = reinterpret_cast<int*>(uintptr_t(player) + 0x31F8);
	auto buttons_changed = buttons ^ *buttons_last;
	*reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x31EC) = *buttons_last;
	*buttons_last = buttons;
	*reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x31F0) = buttons & buttons_changed;
	*reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x31F4) = buttons_changed & ~buttons;

	g_pPrediction->CheckMovingOnGround(player, g_pGlobalVars->frametime);

	player->set_local_view_angles(cmd->viewangles);

	player->physics_run_think(0) ? player->pre_think() : 0;

	auto next_think = reinterpret_cast<int*>(uintptr_t(player) + 0xFC);
	if (*next_think
		&& *next_think != -1
	 	&& *next_think <= player->get_tickbase()) {
		*next_think = -1;

		static const auto unknown_fn = reinterpret_cast<void(__thiscall*)(int)>(SIG("client_panorama.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B"));
		unknown_fn(0);

		player->think();
	}

	g_pPrediction->SetupMove(player, cmd, g_pMoveHelper, reinterpret_cast<CMoveData*>(move_data));

	if (vehicle)
		utils::get_vfunc<void(__thiscall*)(int, C_CSPlayer*, CMoveData*)>(vehicle, 5)(uintptr_t(vehicle), player, reinterpret_cast<CMoveData*>(move_data));
	else
		g_pMovement->ProcessMovement(player, reinterpret_cast<CMoveData*>(move_data));

	g_pPrediction->FinishMove(player, cmd, reinterpret_cast<CMoveData*>(move_data));
	g_pMoveHelper->ProcessImpacts();

	post_think(player);

	g_pPrediction->m_bInPrediction = backup_in_prediction;
	g_pPrediction->m_bFirstTimePredicted = backup_is_first_time_predicted;
}

void c_prediction::end(C_CSPlayer* player, CUserCmd* cmd) {
    if (!player 
        || g_pEngine->IsPlayingDemo()
		|| !player->is_alive())
		return;

	g_pMovement->FinishTrackPredictionErrors(player);
	g_pMoveHelper->SetHost(nullptr);
	g_pMovement->Reset();

	*reinterpret_cast<uint32_t*>(uintptr_t(player) + 0x3338) = 0;
	*prediction_random_seed = -1;
	*prediction_player = 0;

	!g_pPrediction->m_bEnginePaused && g_pGlobalVars->frametime ? player->get_tickbase()++ : 0;

	g_pGlobalVars->curtime = backup_curtime;
	g_pGlobalVars->frametime = backup_frametime;;
}
