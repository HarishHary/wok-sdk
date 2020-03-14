#include "sdk/sdk.h"
#include "hooks/hooks.h"

IBaseClientDll*			g_pClientDll = nullptr;
IClientEntityList*		g_pEntityList = nullptr;
IVEngineClient*			g_pEngine = nullptr;
IVModelInfo*			g_pModelInfo = nullptr;
ISurface*				g_pSurface = nullptr;
IVEngineVGui*			g_pEngineVGui = nullptr;
IClientMode*			g_pClientMode = nullptr;
CClientState*			g_pClientState = nullptr;
CGlobalVarsBase*		g_pGlobalVars = nullptr;
CInput*					g_pInput = nullptr;
IPrediction*			g_pPrediction = nullptr;
IMoveHelper*			g_pMoveHelper = nullptr;
IGameMovement*			g_pMovement = nullptr;
IMemAlloc*				g_pMemAlloc = nullptr;
IVDebugOverlay*			g_pDebugOverlay = nullptr;
ICVar*					g_pCVar = nullptr;
IGameEventManager*		g_pEventManager = nullptr;
IEngineTrace*			g_pTrace = nullptr;
IPhysicsSurfaceProps*	g_pSurfaceData = nullptr;
IInputSystem*			g_pInputSystem = nullptr;
IClientLeafSystem*		g_pClientLeafSystem = nullptr;
CGameRules**			g_pGameRules = nullptr;
IMDLCache*				g_pModelCache = nullptr;
IVModelRender*			g_pModelRender = nullptr;
IMaterialSystem*		g_pMaterialSystem = nullptr;
IVRenderView*			g_pRenderView = nullptr;
CGlowObjectManager*		g_pGlowManager = nullptr;
IViewRenderBeams*		g_pViewRenderBeams = nullptr;
IWeaponSystem*			g_pWeaponSystem = nullptr;
CFontManager*			g_pFontManager = nullptr;
C_CSPlayerResource** 	g_pPlayerResource = nullptr;

namespace wok {
	namespace interfaces {
		void init() {
			g_pClientDll = memory::capture_interface<IBaseClientDll>(_("client_panorama.dll"), _("VClient018"));
			g_pEntityList = memory::capture_interface<IClientEntityList>(_("client_panorama.dll"), _("VClientEntityList003"));
			g_pEngine = memory::capture_interface<IVEngineClient>(_("engine.dll"), _("VEngineClient014"));
			g_pModelInfo = memory::capture_interface<IVModelInfo>(_("engine.dll"), _("VModelInfoClient004"));
			g_pSurface = memory::capture_interface<ISurface>(_("vguimatsurface.dll"), _("VGUI_Surface031"));
			g_pEngineVGui = memory::capture_interface<IVEngineVGui>(_("engine.dll"), _("VEngineVGui001"));
			g_pClientMode = **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5);
			g_pClientState = **reinterpret_cast<CClientState***>((*reinterpret_cast<uintptr_t**>(g_pEngine))[12] + 0x10);
			g_pGlobalVars = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0] + 0x1B);
			g_pInput = *reinterpret_cast<CInput**>(SIG("client_panorama.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 0x1);
			g_pPrediction = memory::capture_interface<IPrediction>(_("client_panorama.dll"), _("VClientPrediction001"));
			g_pMoveHelper = **reinterpret_cast<IMoveHelper***>(SIG("client_panorama.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 0x2);
			g_pMovement = memory::capture_interface<IGameMovement>(_("client_panorama.dll"), _("GameMovement001"));
			g_pMemAlloc = *reinterpret_cast<IMemAlloc**>(GetProcAddress(GetModuleHandleA(_("tier0.dll")), _("g_pMemAlloc")));
			g_pDebugOverlay = memory::capture_interface<IVDebugOverlay>(_("engine.dll"), _("VDebugOverlay004"));
			g_pCVar = memory::capture_interface<ICVar>(_("vstdlib.dll"), _("VEngineCvar007"));
			g_pEventManager = memory::capture_interface<IGameEventManager>(_("engine.dll"), _("GAMEEVENTSMANAGER002"));
			g_pTrace = memory::capture_interface<IEngineTrace>(_("engine.dll"), _("EngineTraceClient004"));
			g_pSurfaceData = memory::capture_interface<IPhysicsSurfaceProps>(_("vphysics.dll"), _("VPhysicsSurfaceProps001"));
			g_pInputSystem = memory::capture_interface<IInputSystem>(_("inputsystem.dll"), _("InputSystemVersion001"));
			g_pClientLeafSystem = memory::capture_interface<IClientLeafSystem>(_("client_panorama.dll"), _("ClientLeafSystem002"));
			g_pGameRules = *reinterpret_cast<CGameRules***>(SIG("client_panorama.dll", "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A") + 0x1);
			g_pModelCache = memory::capture_interface<IMDLCache>(_("datacache.dll"), _("MDLCache004"));
			g_pModelRender = memory::capture_interface<IVModelRender>(_("engine.dll"), _("VEngineModel016"));
			g_pMaterialSystem = memory::capture_interface<IMaterialSystem>(_("materialsystem.dll"), _("VMaterialSystem080"));
			g_pRenderView = memory::capture_interface<IVRenderView>(_("engine.dll"), _("VEngineRenderView014"));
			g_pGlowManager = *reinterpret_cast<CGlowObjectManager**>(SIG("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 0x3);
			g_pViewRenderBeams = *reinterpret_cast<IViewRenderBeams**>(SIG("client_panorama.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 0x1);
			g_pWeaponSystem = *reinterpret_cast<IWeaponSystem**>(SIG("client_panorama.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 0x2);
			g_pFontManager = *reinterpret_cast<CFontManager**>(SIG("vguimatsurface.dll", "74 1D 8B 0D ? ? ? ? 68 ? ? ? ?") + 0x4);
			g_pPlayerResource = *reinterpret_cast<C_CSPlayerResource***>(SIG("client_panorama.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 0x2);

			g::local = *reinterpret_cast<C_LocalPlayer*>(SIG("client_panorama.dll", "8B 0D ? ? ? ? 83 FF FF 74 07") + 0x2);
		}
	}

	void init() {
		while (!(hooks::csgo_window = FindWindowA(_("Valve001"), nullptr)))
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

		hooks::original_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hooks::csgo_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::wnd_proc)));

		while (!GetModuleHandleA(_("serverbrowser.dll")))
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

		interfaces::init();

		hooks::init();

		c_event::instance();
		c_render::instance();
	}

	void detach() { hooks::undo(); }
}