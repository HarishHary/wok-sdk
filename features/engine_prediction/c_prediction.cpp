#include "c_prediction.h"
#include "../features.h"

void c_prediction::run(C_CSPlayer* player, CUserCmd* cmd) {
	if (!player 
        || g_pEngine->IsPlayingDemo()
		|| !player->is_alive())
		return;

	if (!prediction_random_seed || !prediction_player) {
		prediction_random_seed = *reinterpret_cast<int**>(SIG("client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86") + 0x1);
		prediction_player = *reinterpret_cast<int**>(SIG("client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48") + 0x2);
	}

	g_pPrediction->Update(g_pClientState->m_nDeltaTick, g_pClientState->m_nDeltaTick > 0,
		g_pClientState->last_command_ack, g_pClientState->lastoutgoingcommand + g_pClientState->chokedcommands);

	backup_curtime = g_pGlobalVars->curtime;
	backup_frametime = g_pGlobalVars->frametime;

	bool backup_in_prediction = *reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0x8);
	bool backup_is_first_time_predicted = *reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0x18);

	*reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0x8) = true;
	*reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0x18) = false;

	g_pGlobalVars->curtime = TICKS_TO_TIME(player->get_tickbase());
	g_pGlobalVars->frametime = *reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0xA) ? 0.f : g_pGlobalVars->interval_per_tick;

	*reinterpret_cast<CUserCmd**>(reinterpret_cast<uint32_t>(player) + 0x3334) = cmd;
	*prediction_random_seed = cmd ? MD5_PseudoRandom(cmd->commandnumber) & 0x7FFFFFFF : -1;
	*prediction_player = reinterpret_cast<int>(player);

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
		*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(player) + 0x31FC) = cmd->impulse;

	cmd->buttons |= *reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(player) + 0x3330);
	int m_nButtons = cmd->buttons;
	int* m_afButtonLast = reinterpret_cast<int*>(reinterpret_cast<uint32_t>(player) + 0x31F8);
	int buttonsChanged = m_nButtons ^ *m_afButtonLast;
	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(player) + 0x31EC) = *m_afButtonLast;
	*m_afButtonLast = m_nButtons;
	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(player) + 0x31F0) = m_nButtons & buttonsChanged;
	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(player) + 0x31F4) = buttonsChanged & ~m_nButtons;

	g_pPrediction->CheckMovingOnGround(player, g_pGlobalVars->frametime);
	player->set_local_view_angles(cmd->viewangles);

	if (player->physics_run_think(0))
		player->pre_think();

	auto next_think = reinterpret_cast<int*>(reinterpret_cast<uint32_t>(player) + 0xFC);
	if (*next_think > 0 && *next_think <= player->get_tickbase()) {
		*next_think = -1;
		player->think();
	}

	memset(&movedata, 0, sizeof(CMoveData));
	g_pPrediction->SetupMove(player, cmd, g_pMoveHelper, &movedata);

	if (vehicle)
		utils::get_vfunc<void(__thiscall*)(int, C_BaseEntity*, void*)>(vehicle, 5)(reinterpret_cast<uint32_t>(vehicle), player, &movedata);
	else
		g_pMovement->ProcessMovement(player, &movedata);

	g_pPrediction->FinishMove(player, cmd, &movedata);

	g_pMoveHelper->ProcessImpacts();

	post_think(player);

	*reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0x8) = backup_in_prediction;
	*reinterpret_cast<bool*>(reinterpret_cast<uint32_t>(g_pPrediction) + 0x18) = backup_is_first_time_predicted;
}

void c_prediction::end(C_CSPlayer* player, CUserCmd* cmd) {
    if (!player 
        || g_pEngine->IsPlayingDemo()
		|| !player->is_alive())
		return;

	g_pMovement->FinishTrackPredictionErrors(player);
	g_pMoveHelper->SetHost(nullptr);

	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(player) + 0x3334) = 0;
	*prediction_random_seed = 0;
	*prediction_player = 0;

	g_pMovement->Reset();

	g_pGlobalVars->curtime = backup_curtime;
	g_pGlobalVars->frametime = backup_frametime;;
}