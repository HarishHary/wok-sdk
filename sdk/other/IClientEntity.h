#pragma once

struct SpatializationInfo_t;

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable {
public:
	virtual void             Release() = 0;
	virtual const Vector     GetAbsOrigin() const = 0;
	virtual const QAngle     GetAbsAngles() const = 0;
	virtual void*            GetMouth() = 0;
	virtual bool             GetSoundSpatialization(SpatializationInfo_t info) = 0;
	virtual bool             IsBlurred() = 0;
};