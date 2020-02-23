#pragma once

class bf_read;
class bf_write;

class CUserCmd {
public:
	CUserCmd() {
		memset(this, 0, sizeof(*this));
	};
	virtual ~CUserCmd() {};

	CRC32_t GetChecksum() const {
		CRC32_t crc;
		CRC32_Init(&crc);

		CRC32_ProcessBuffer(&crc, &commandnumber, sizeof(commandnumber));
		CRC32_ProcessBuffer(&crc, &tickcount, sizeof(tickcount));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

		CRC32_Final(&crc);
		return crc;
	}

	int     commandnumber;      // 0x04 For matching server and client commands for debugging
	int     tickcount;          // 0x08 the tick the client created this command
	QAngle  viewangles;         // 0x0C Player instantaneous view angles.
	Vector  aimdirection;       // 0x18
	float   forwardmove;        // 0x24
	float   sidemove;           // 0x28
	float   upmove;             // 0x2C
	int     buttons;            // 0x30 Attack button states
	char    impulse;            // 0x34
	int     weaponselect;       // 0x38 Current weapon id
	int     weaponsubtype;      // 0x3C
	int     random_seed;        // 0x40 For shared random functions
	short   mousedx;            // 0x44 mouse accum in x from create move
	short   mousedy;            // 0x46 mouse accum in y from create move
	bool    hasbeenpredicted;   // 0x48 Client only, tracks whether we've predicted this command at least once
	char    pad_0x4C[0x18];     // 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

class CVerifiedUserCmd {
public:
	CUserCmd  m_cmd;
	CRC32_t   m_crc;
};

class CInput {
public:
	char                pad_0x00[0x0C];
	bool                m_trackir_available;
	bool                m_mouse_initiated;
	bool                m_mouse_active;
	bool                m_fJoystickAdvancedInit;
	char                pad_0x08[0x2C];
	void*               m_pKeys;
	char                pad_0x38[0x6C];
	bool                m_fCameraInterceptingMouse;
	bool                m_fCameraInThirdPerson;
	bool                m_fCameraMovingWithMouse;
	Vector				m_vecCameraOffset;
	bool                m_fCameraDistanceMove;
	int                 m_nCameraOldX;
	int                 m_nCameraOldY;
	int                 m_nCameraX;
	int                 m_nCameraY;
	bool                m_CameraIsOrthographic;
	Vector              m_angPreviousViewAngles;
	Vector              m_angPreviousViewAnglesTilt;
	float               m_flLastForwardMove;
	int                 m_nClearInputState;
	char                pad_0xE4[0x8];
	CUserCmd*           m_pCommands;
	CVerifiedUserCmd*   m_pVerifiedCommands;

	VFUNC(GetUserCmd(int sequence_number), 8, CUserCmd*(__thiscall*)(void*, int, int), 0, sequence_number);
	VFUNC(GetUserCmd(int slot, int sequence_number), 8, CUserCmd*(__thiscall*)(void*, int, int), slot, sequence_number);
	CVerifiedUserCmd* GetVerifiedCmd(int sequence_number) {
		return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
	}
};
extern CInput* g_pInput;