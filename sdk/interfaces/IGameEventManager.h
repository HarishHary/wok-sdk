#pragma once

class IGameEvent {
public:
	virtual ~IGameEvent() = 0;
	virtual const char* GetName() const = 0;

	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char *keyName = NULL) = 0;

	virtual bool GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long GetUint64(const char *keyName = NULL, unsigned long defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;
	virtual const wchar_t* GetWString(const char *keyName, const wchar_t *defaultValue = L"") = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetUint64(const char *keyName, unsigned long value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
	virtual void SetWString(const char *keyName, const wchar_t *value) = 0;
};

class IGameEventListener {
public:
	virtual ~IGameEventListener() {}
	virtual void FireGameEvent(IGameEvent* Event) = 0;
	virtual int GetEventDebugID() {
		return EVENT_DEBUG_ID_INIT;
	}
};

#undef CreateEvent
class IGameEventManager {
public:
	virtual ~IGameEventManager() {};
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener* listener, const char* name, bool bServerSide) = 0;
	virtual bool FindListener(IGameEventListener* listener, const char* name) = 0;
	virtual void RemoveListener(IGameEventListener* listener) = 0;
	virtual void AddListenerGlobal(IGameEventListener* listener, bool bIsServerSide) = 0;
	virtual IGameEvent* CreateEvent(const char* name, bool bForce = false, int* pCookie = NULL) = 0;
	virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent* event) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
	virtual void FreeEvent(IGameEvent* event) = 0;
	virtual bool SerializeEvent(IGameEvent* event, bf_write* buf) = 0;
	virtual IGameEvent* UnserializeEvent(bf_read* buf) = 0;
	virtual void *GetEventDataTypes(IGameEvent* event) = 0;
};
extern IGameEventManager* g_pEventManager;