#include "c_movement.h"
#include "../features.h"

void c_movement::bhop(){
	if (!menu->config.misc_bhop 
		|| !g::local
		|| !g_pEngine->IsInGame()
		|| !g_pEngine->IsConnected()
		|| !g::local->is_alive()
		|| g::local->get_water_level() >= 2
		|| IN_RANGE(g::local->get_move_type(), MOVETYPE_NOCLIP, MOVETYPE_OBSERVER)
		|| g::local->get_ground_entity().IsValid()
		|| !(g::cmd->buttons & IN_JUMP))
		return;

	g::cmd->buttons &= ~IN_JUMP;
}

void c_movement::autostrafe(){
	if (!menu->config.misc_autostrafe 
		|| !g::local
		|| !g_pEngine->IsInGame()
		|| !g_pEngine->IsConnected()
		|| !g::local->is_alive()
		|| g::local->get_water_level() >= 2
		|| IN_RANGE(g::local->get_move_type(), MOVETYPE_NOCLIP, MOVETYPE_OBSERVER)
		|| g::local->get_ground_entity().IsValid()
		|| !(g::cmd->buttons & IN_JUMP))
		return;

    g::cmd->move.y = g::cmd->commandnumber % 2 ? 450.f : -450.f;
}