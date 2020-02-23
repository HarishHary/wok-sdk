#pragma once

enum SolidType {
	SOLID_NONE = 0,
	SOLID_BSP = 1,
	SOLID_BBOX = 2,
	SOLID_OBB = 3,
	SOLID_OBB_YAW = 4,
	SOLID_CUSTOM = 5,
	SOLID_VPHYSICS = 6,
	SOLID_LAST
};

struct Model;
struct Ray_t;
class CGameTrace;

class ICollideable {
public:
	virtual IHandleEntity* GetEntityHandle() = 0;
	virtual Vector& OBBMins() const = 0;
	virtual Vector& OBBMaxs() const = 0;
	virtual void WorldSpaceTriggerBounds(Vector* mins, Vector* maxs) const = 0;
	virtual bool TestCollision(const Ray_t& ray, unsigned int mask, CGameTrace& trace) = 0;
	virtual bool TestHitboxes(const Ray_t& ray, unsigned int mask, CGameTrace& trace) = 0;
	virtual int GetCollisionModelIndex() = 0;
	virtual const Model* GetCollisionModel() = 0;
	virtual Vector&	GetCollisionOrigin() const = 0;
	virtual Vector& GetCollisionAngles() const = 0;
	virtual const matrix3x4_t& CollisionToWorldTransform() const = 0;
	virtual SolidType GetSolid() const = 0;
	virtual int GetSolidFlags() const = 0;
	virtual IClientUnknown* GetClientUnknown() = 0;
	virtual int GetCollisionGroup() const = 0;
	virtual void WorldSpaceSurroundingBounds(Vector* mins, Vector* maxs) = 0;
	virtual bool ShouldTouchTrigger(int flags) const = 0;
	virtual const matrix3x4_t* GetRootParentToWorldTransform() const = 0;
};

class C_BaseEntity;
class IClientEntity;
class IClientAlphaProperty;
class IClientModelRenderable;

class IClientUnknown : public IHandleEntity {
public:
	virtual ICollideable*              GetCollideable() = 0;
	virtual IClientNetworkable*        GetClientNetworkable() = 0;
	virtual IClientRenderable*         GetClientRenderable() = 0;
	virtual IClientEntity*             GetIClientEntity() = 0;
	virtual C_BaseEntity*              GetBaseEntity() = 0;
	virtual IClientThinkable*          GetClientThinkable() = 0;
	virtual IClientModelRenderable*    GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};