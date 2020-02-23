#pragma once

class C_BaseEntity;
class IHandleEntity;

class VectorAligned : public Vector {
public:
	VectorAligned() {}

	VectorAligned(const Vector &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	float w;
};

struct Ray_t {
	Ray_t() {}

	VectorAligned		m_Start;
	VectorAligned		m_dt;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const matrix3x4_t	*m_pWorldAxisTransform;
	bool				m_IsRay;
	bool				m_IsSwept;

	void Init(Vector vecStart, Vector vecEnd) {
		m_dt = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_dt.LengthSqr() != 0);
		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}
	void Init(Vector vecStart, Vector vecEnd, const Vector & extents) {
		m_dt = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_dt.LengthSqr() != 0);
		m_Extents = extents;
		m_pWorldAxisTransform = NULL;
		m_IsRay = false;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}
	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_dt = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_dt.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

struct csurface_t {
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

struct cplane_t {
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter {
public:
	CTraceFilter() = default;
	explicit CTraceFilter(IHandleEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING) {
		pSkip = entity;
		type = tracetype;
	}
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip);
	}

	TraceType_t	GetTraceType() const {
		return type;
	}
	IHandleEntity* pSkip;
	TraceType_t type = TRACE_EVERYTHING;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter {
public:
	CTraceFilterSkipTwoEntities() = default;
	CTraceFilterSkipTwoEntities(IHandleEntity* pPassEnt1, IHandleEntity* pPassEnt2) {
		pPassEntity1 = pPassEnt1;
		pPassEntity2 = pPassEnt2;
	}

	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pPassEntity1 || pEntityHandle == pPassEntity2);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	IHandleEntity* pPassEntity1;
	IHandleEntity* pPassEntity2;
};

class CTraceFilterWorldOnly : public ITraceFilter {
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		return false;
	}
	TraceType_t	GetTraceType() const {
		return TRACE_WORLD_ONLY;
	}
};

class CTraceFilterNoPlayers : public CTraceFilter {
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) override {
		return false;
	}
};

class CTraceEntity : public ITraceFilter {
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int contentsMask) {
		return (pEntityHandle == pHit);
	}
	TraceType_t	GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}
	IHandleEntity* pHit;
};

class CBaseTrace {
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

class CGameTrace : public CBaseTrace {
public:
	bool DidHitWorld() const { return m_pEnt == (C_BaseEntity*)g_pEntityList->GetClientEntity(0); }
	bool DidHitNonWorldEntity() const { return m_pEnt && !DidHitWorld(); }
	int                     GetEntityIndex() const;
	bool                    DidHit() const { return fraction < 1 || allsolid || startsolid; };
	bool					BoneIsVisible() const;
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	C_BaseEntity*           m_pEnt;
	int                     hitbox;

	CGameTrace() {}
public:
	CGameTrace(const CGameTrace& vOther) {
		fractionleftsolid = vOther.fractionleftsolid;
		surface = vOther.surface;
		hitgroup = vOther.hitgroup;
		physicsbone = vOther.physicsbone;
		worldSurfaceIndex = vOther.worldSurfaceIndex;
		m_pEnt = vOther.m_pEnt;
		hitbox = vOther.hitbox;
		startpos = vOther.startpos;
		endpos = vOther.endpos;
		plane = vOther.plane;
		fraction = vOther.fraction;
		contents = vOther.contents;
		dispFlags = vOther.dispFlags;
		allsolid = vOther.allsolid;
		startsolid = vOther.startsolid;
	}
};