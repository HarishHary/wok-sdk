#pragma once

class CBaseHandle;

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class CBaseHandle {
	friend class C_BaseEntityList;
public:
	CBaseHandle();
	CBaseHandle(const CBaseHandle &other);
	CBaseHandle(unsigned long value);
	CBaseHandle(int iEntry, int iSerialNumber);

	void Init(int iEntry, int iSerialNumber);
	void Term();

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool IsValid() const;

	int GetEntryIndex() const;
	int GetSerialNumber() const;

	int ToInt() const;
	bool operator !=(const CBaseHandle &other) const;
	bool operator ==(const CBaseHandle &other) const;
	bool operator ==(const IHandleEntity* pEnt) const;
	bool operator !=(const IHandleEntity* pEnt) const;
	bool operator <(const CBaseHandle &other) const;
	bool operator <(const IHandleEntity* pEnt) const;

	// Assign a value to the handle.
	const CBaseHandle& operator=(const IHandleEntity *pEntity);
	const CBaseHandle& Set(const IHandleEntity *pEntity);

	IHandleEntity* Get() const;
protected:
	unsigned long  m_Index;
};

inline CBaseHandle::CBaseHandle() { m_Index = INVALID_EHANDLE_INDEX; }

inline CBaseHandle::CBaseHandle(const CBaseHandle &other) { m_Index = other.m_Index; }

inline CBaseHandle::CBaseHandle(unsigned long value) { m_Index = value; }

inline CBaseHandle::CBaseHandle(int iEntry, int iSerialNumber) { Init(iEntry, iSerialNumber); }

inline void CBaseHandle::Init(int iEntry, int iSerialNumber) { m_Index = (unsigned long)(iEntry | (iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS)); }

inline void CBaseHandle::Term() { m_Index = INVALID_EHANDLE_INDEX; }

inline bool CBaseHandle::IsValid() const { return m_Index != INVALID_EHANDLE_INDEX; }

inline int CBaseHandle::GetEntryIndex() const {
	// There is a hack here: due to a bug in the original implementation of the 
	// entity handle system, an attempt to look up an invalid entity index in 
	// certain cirumstances might fall through to the the mask operation below.
	// This would mask an invalid index to be in fact a lookup of entity number
	// NUM_ENT_ENTRIES, so invalid ent indexes end up actually looking up the
	// last slot in the entities array. Since this slot is always empty, the 
	// lookup returns NULL and the expected behavior occurs through this unexpected
	// route.
	// A lot of code actually depends on this behavior, and the bug was only exposed
	// after a change to NUM_SERIAL_NUM_BITS increased the number of allowable
	// static props in the world. So the if-stanza below detects this case and 
	// retains the prior (bug-submarining) behavior.
	if (!IsValid())
		return NUM_ENT_ENTRIES - 1;

	return m_Index & ENT_ENTRY_MASK;
}

inline int CBaseHandle::GetSerialNumber() const { return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS; }

inline int CBaseHandle::ToInt() const { return (int)m_Index; }

inline bool CBaseHandle::operator !=(const CBaseHandle &other) const { return m_Index != other.m_Index; }

inline bool CBaseHandle::operator ==(const CBaseHandle &other) const { return m_Index == other.m_Index; }

inline bool CBaseHandle::operator ==(const IHandleEntity* pEnt) const { return Get() == pEnt; }

inline bool CBaseHandle::operator !=(const IHandleEntity* pEnt) const { return Get() != pEnt; }

inline bool CBaseHandle::operator <(const CBaseHandle &other) const { return m_Index < other.m_Index; }

inline bool CBaseHandle::operator <(const IHandleEntity *pEntity) const {
	unsigned long otherIndex = (pEntity) ? pEntity->GetRefEHandle().m_Index : INVALID_EHANDLE_INDEX;
	return m_Index < otherIndex;
}

inline const CBaseHandle& CBaseHandle::operator=(const IHandleEntity *pEntity) { return Set(pEntity); }

inline const CBaseHandle& CBaseHandle::Set(const IHandleEntity *pEntity) {
	if (pEntity)
		*this = pEntity->GetRefEHandle();
	else
		m_Index = INVALID_EHANDLE_INDEX;

	return *this;
}