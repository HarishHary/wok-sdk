#pragma once

class CMoveData {
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	Vector  m_vecViewAngles;        // Command view angles (local space)
	Vector  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
	Vector  m_vecAngles;            // edict::angles
	Vector  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	Vector  m_outWishVel;           // This is where you tried 
	Vector  m_outJumpVel;           // This is your jump velocity
	Vector  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[5];
	Vector  m_vecAbsOrigin;
};

class CUserCmd;
class C_BasePlayer;
class IMoveHelper;

class IPrediction {
public:
	char pad0[8]; 					// 0x0000
	bool m_bInPrediction;			// 0x0008
	char pad1[1];					// 0x0009
	bool m_bEnginePaused;			// 0x000A
	char pad2[13];					// 0x000B
	bool m_bFirstTimePredicted;		// 0x0018

	VFUNC(Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command), 3, void(__thiscall*)(void*, int, bool, int, int), startframe, validframe, incoming_acknowledged, outgoing_command)
	VFUNC(InPrediction(), 14, bool(__thiscall*)(void*))
	VFUNC(CheckMovingOnGround(C_BasePlayer* player, float frame_time), 18, void(__thiscall*)(void*, C_BasePlayer*, double), player, frame_time)
	VFUNC(RunCommand(C_BasePlayer *player, CUserCmd *cmd, IMoveHelper *movehelper), 19, void(__thiscall*)(void*, C_BasePlayer*, CUserCmd *, IMoveHelper*), player, cmd, movehelper)
	VFUNC(SetupMove(C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* helper, CMoveData* movedata), 20, void(__thiscall*)(void*, C_BasePlayer*, CUserCmd*, IMoveHelper*, CMoveData*), player, cmd, helper, movedata)
	VFUNC(FinishMove(C_BasePlayer* player, CUserCmd* cmd, CMoveData* movedata), 21, void(__thiscall*)(void*, C_BasePlayer*, CUserCmd*, CMoveData*), player, cmd, movedata)
};
extern IPrediction* g_pPrediction;