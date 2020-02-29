#pragma once
#include "../globals.h"
#include "../utils/memory/hook.h"
#include "../features/features.h"

class hooks {
public:
	static void init();
	static void undo();
private:
	inline static c_hook<IClientMode> client_mode_hook;
	inline static c_hook<IVEngineVGui> engine_vgui_hook;

	typedef bool(__stdcall* create_move_t)(float, CUserCmd*);
	inline static create_move_t _create_move;
	static bool __stdcall create_move(float input_sample_time, CUserCmd* cmd);

	typedef void(__thiscall* paint_t) (uintptr_t, int);
	inline static paint_t _paint;
	static void __fastcall paint(uintptr_t ecx, uintptr_t edx, int mode);

	// // // // // // // // // // // // // // // // // // //

	typedef bool(__thiscall* setup_bones_t)(uintptr_t, matrix3x4_t*, int, int, float);
	inline static setup_bones_t _setup_bones;
	static bool __fastcall setup_bones(uintptr_t ecx, uintptr_t edx, matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time);
public:
	inline static HWND csgo_window = nullptr;
	inline static WNDPROC original_wnd_proc = nullptr;
	static LRESULT __stdcall wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

class c_event : public c_singleton<c_event> {
public:
	c_event() { if (!event_listener) { event_listener = new listeners(); } }
	~c_event() { delete event_listener; }
private:
	class listeners : public IGameEventListener {
	public:
		listeners() {
			g_pEventManager->AddListener(this, _("bullet_impact"), false);
			g_pEventManager->AddListener(this, _("weapon_fire"), false);
			g_pEventManager->AddListener(this, _("player_hurt"), false);
			g_pEventManager->AddListener(this, _("player_footstep"), false);
			g_pEventManager->AddListener(this, _("player_jump"), false);
			g_pEventManager->AddListener(this, _("round_prestart"), false);
			g_pEventManager->AddListener(this, _("round_end"), false);
			g_pEventManager->AddListener(this, _("round_start"), false);
			g_pEventManager->AddListener(this, _("round_freeze_end"), false);
			g_pEventManager->AddListener(this, _("player_death"), false);
			g_pEventManager->AddListener(this, _("item_purchase"), false);
			g_pEventManager->AddListener(this, _("cs_game_disconnected"), false);
		}
		~listeners() {
			g_pEventManager->RemoveListener(this);
		}

		std::string hitgroup_to_name(int hitgroup) {
			switch (hitgroup) {
			case HITGROUP_HEAD: return _("HEAD");
			case HITGROUP_LEFTLEG: return _("LEFT LEG");
			case HITGROUP_RIGHTLEG: return _("RIGHT LEG");
			case HITGROUP_STOMACH: return _("STOMACH");
			case HITGROUP_LEFTARM: return _("LEFT ARM");
			case HITGROUP_RIGHTARM: return _("RIGHT ARM");
			case HITGROUP_GENERIC: return _("GENERIC");
			default: return _("BODY");
			}
		}

		virtual void FireGameEvent(IGameEvent *event);

		virtual int GetEventDebugID() {
			return EVENT_DEBUG_ID_INIT;
		}
	};
public:
	listeners* event_listener = nullptr;
};
