#pragma once
#include "../common_includes.h"

#include "definitions.h"
#include "datatypes/Vector.h"
#include "datatypes/Vector2D.h"
#include "datatypes/QAngle.h"
#include "datatypes/matrix3x4_t.h"
#include "datatypes/CUtlVector.h"
#include "datatypes/Color.h"
#include "datatypes/VMatrix.h"

#include "definitions.h"
#include "entity_structs.h"

#include "other/CBaseHandle.h"
#include "other/ClientClass.h"
#include "other/IClientNetworkable.h"
#include "other/IClientRenderable.h"
#include "other/IClientThinkable.h"
#include "other/IClientUnknown.h"
#include "other/IClientEntity.h"
#include "../utils/utils.h"

#include "other/ConVar.h"
#include "other/CRC32_t.h"
#include "other/IMaterial.h"
#include "other/IMaterialVar.h"
#include "other/INetChannelInfo.h"

#include "interfaces/IClientEntityList.h"
#include "other/ITraceFilter.h"

#include "interfaces/IBaseClientDll.h"
#include "interfaces/IVEngineClient.h"
#include "interfaces/IVModelInfo.h"
#include "interfaces/ISurface.h"
#include "interfaces/IVEngineVGui.h"
#include "interfaces/IClientMode.h"
#include "interfaces/CClientState.h"
#include "interfaces/CGlobalVarsBase.h"
#include "interfaces/CInput.h"
#include "interfaces/IPrediction.h"
#include "interfaces/IMoveHelper.h"
#include "interfaces/IGameMovement.h"
#include "interfaces/IMemAlloc.h"
#include "interfaces/IVDebugOverlay.h"
#include "interfaces/ICvar.h"
#include "interfaces/IGameEventManager.h"
#include "interfaces/IEngineTrace.h"
#include "interfaces/IPhysicsSurfaceProps.h"
#include "interfaces/IInputSystem.h"
#include "interfaces/IClientLeafSystem.h"
#include "interfaces/CGameRules.h"
#include "interfaces/IMDLCache.h"
#include "interfaces/IVModelRender.h"
#include "interfaces/IMaterialSystem.h"
#include "interfaces/IVRenderView.h"
#include "interfaces/CGlowObjectManager.h"
#include "interfaces/IViewRenderBeams.h"

#include "../utils/render/render.h"
#include "../utils/math/math.h"

#include "entity_classes.h"
#include "../globals.h"

namespace wok {
	namespace interfaces {
		void init();
	}

	void init();
	void detach();
}