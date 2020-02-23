#pragma once

class C_BaseEntity;
class CBaseHandle;
class IClientEntity;

class IClientEntityListener {
public:
	virtual void OnEntityCreated(C_BaseEntity* pEntity) {}
	virtual void OnEntityDeleted(C_BaseEntity* pEntity) {}
};

class IClientNetworkable;

class IClientEntityList {
public:
	virtual IClientNetworkable*   GetClientNetworkable(int entnum) = 0;
	virtual void*                 unused0() = 0;
	virtual void*                 unused1() = 0;
	virtual C_BaseEntity*         GetClientEntity(int entNum) = 0;
	virtual IClientEntity*        GetClientEntityFromHandle(CBaseHandle hEnt) = 0;
	virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int                   GetHighestEntityIndex() = 0;
	virtual void                  SetMaxEntities(int maxEnts) = 0;
	virtual int                   GetMaxEntities() = 0;
};
extern IClientEntityList*  g_pEntityList;