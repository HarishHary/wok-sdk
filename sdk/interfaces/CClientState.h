#pragma once

class INetMsg {
public:
	virtual	~INetMsg() = default;
	virtual void SetNetChannel(void* netchan) = 0;
	virtual void SetReliable(bool state) = 0;
	virtual bool Process() = 0;
	virtual	bool ReadFromBuffer(void* buffer) = 0;
	virtual	bool WriteToBuffer(void* buffer) = 0;
	virtual bool IsReliable() const = 0;
	virtual int	GetType() const = 0;
	virtual int	GetGroup() const = 0;
	virtual const char* GetName() const = 0;
	virtual void* GetNetChannel() const = 0;
	virtual const char* ToString() const = 0;
};

template<typename T>
class CNetMessagePB : public INetMsg, public T {};

class CCLCMsg_Move {
private:
	char pad0[8];
public:
	int numBackupCommands;
	int numNewCommands;
};

using CCLCMsg_Move_t = CNetMessagePB<CCLCMsg_Move>;

class INetChannel {
public:
	char pad0[20];				 //0x0000
	bool m_bProcessingMessages;  //0x0014
	bool m_bShouldDelete;        //0x0015
	char pad1[2];				 //0x0016
	int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
	int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
	int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
	int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
	int32_t m_nChokedPackets;    //0x002C number of choked packets
	char pad2[1044];			 //0x0030
}; //Size: 0x0444

class CClockDriftMgr {
public:
	float m_ClockOffsets[16];   //0x0000
	uint32_t m_iCurClockOffset; //0x0044
	uint32_t m_nServerTick;     //0x0048
	uint32_t m_nClientTick;     //0x004C
}; 

class CEventInfo {
public:
	char pad0[4];
	float m_Delay;
	char pad1[4];
	ClientClass* m_ClientClass;
	char pad2[40];
	CEventInfo* m_Next;
};

class CClientState {
public:
	char pad0[156];					//0x0000
	INetChannel* m_NetChannel;      //0x009C
	uint32_t m_nChallengeNr;        //0x00A0
	char pad1[100];					//0x00A4
	uint32_t m_nSignonState;        //0x0108
	char pad2[8];					//0x010C
	float m_flNextCmdTime;          //0x0114
	uint32_t m_nServerCount;        //0x0118
	uint32_t m_nCurrentSequence;    //0x011C
	char pad3[8];					//0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0128
	uint32_t m_nDeltaTick;          //0x0174
	bool m_bPaused;                 //0x0178
	char pad4[3];					//0x017D
	uint32_t m_nViewEntity;         //0x0180
	uint32_t m_nPlayerSlot;         //0x0184
	char m_szLevelName[260];        //0x0188
	char m_szLevelNameShort[80];    //0x028C
	char m_szGroupName[80];         //0x02DC
	char pad5[92];					//0x032С
	uint32_t m_nMaxClients;         //0x0388
	char pad6[18824];				//0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad7[3];					//0x4C9D
	uint32_t oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	int lastoutgoingcommand;        //0x4CAC
	int chokedcommands;             //0x4CB0
	int last_command_ack;           //0x4CB4
	int command_ack;                //0x4CB8
	int m_nSoundSequence;           //0x4CBC
	char pad8[80];					//0x4CC0
	QAngle viewangles;              //0x4D10
	char pad9[208];					//0x4D1C
};
extern CClientState* g_pClientState;