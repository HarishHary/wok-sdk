#include "hooks.h"

void hooks::init() { 
	client_mode_hook.setup(g_pClientMode);
	engine_vgui_hook.setup(g_pEngineVGui);

	_create_move = client_mode_hook.apply<create_move_t>(24, create_move);

	_paint = engine_vgui_hook.apply<paint_t>(14, paint);

	// // // // // // // // // // // // // // // // // // //

	static const auto c_cs_player_vtable = *reinterpret_cast<uint32_t**>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x47);
	static const auto c_cs_player_renderable_vtable = *reinterpret_cast<uint32_t**>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x4B + 0x3);

	static c_bphook c_cs_player_hook(c_cs_player_vtable);
	static c_bphook c_cs_player_renderable_hook(c_cs_player_renderable_vtable);

	_setup_bones = c_cs_player_renderable_hook.apply<setup_bones_t>(13, setup_bones);
}

void hooks::undo() {
	client_mode_hook.unapply(24);
	engine_vgui_hook.unapply(14);
}

LRESULT hooks::wnd_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	return CallWindowProcA(original_wnd_proc, hwnd, umsg, wparam, lparam);
}

void c_event::listeners::FireGameEvent(IGameEvent* event) {

};