#ifndef CCAMERA_H_INCL
#define CCAMERA_H_INCL

#include "CObject.h"

class CCamera : public CObject
{
public:
	CCamera(const vec3& focus, float ratio, float fov,
		float near_ = 0.5f, float far_ = 5000.f,
		CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3()) :
		CObject(parent, pos, rot), Focus(focus), Ratio(ratio), FOV(fov),
		Near(near_), Far(far_), WindowDirty(true)
	{
	}

	virtual ~CCamera()
	{
	}

	vec3 getFocus() const { return Focus; }
	float getRatio() const { return Ratio; }
	float getFOV() const { return FOV; }
	float getNear() const { return Near; }
	float getFar() const { return Far; }

	virtual void setFocus(const vec3& focus) { Focus = focus; }
	void setFOV(float fov) { FOV = fov; recalculateProjection(); }
	void setNear(float near_) { Near = near_; recalculateProjection(); }
	void setFar(float far_) { Far = far_; recalculateProjection(); }

	void render();

	void recalculateProjection();

private:
	vec3 Focus;
	float Ratio;
	float FOV;
	float Near;
	float Far;
	bool WindowDirty; // changed width/height, need to recalc projection
};

#endif