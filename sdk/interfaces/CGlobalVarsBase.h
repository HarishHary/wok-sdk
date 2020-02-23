#pragma once

class CGlobalVarsBase {
public:
public:
	float				realtime;
	int					framecount;
	float				absoluteframetime;
	float				absoluteframestarttimestddev;
	float				curtime;
	float				frametime;
	int					maxClients;
	int					tickcount;
	float				interval_per_tick;
	float				interpolation_amount;
	int					simticks_this_frame;
	int					network_protocol;
	void*				pSaveData;
private:
	bool				m_bClient;
public:
	bool				m_bRemoteClient;
private:
	int					nTimestampNetworkingBase;
	int					nTimestampRandomizeWindow;
};
extern CGlobalVarsBase* g_pGlobalVars;