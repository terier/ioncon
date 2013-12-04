#ifndef OBJECT_H_INCL
#define OBJECT_H_INCL

#include <list>
#include "utils.h"
#include "mat4.h"
#include "opengl.h"

class CObject;

typedef std::list<CObject*> ObjectList;
typedef ObjectList::iterator ObjectIter;

class CObject
{
public:
	CObject(CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3());
	virtual ~CObject();

	CObject* getParent() const { return Parent; }
	vec3 getPosition() const { return Position; }
	vec3 getRotation() const { return Rotation; }
	uint getTexture() const { return Texture; }

	void setParent(CObject* parent);
	void setPosition(const vec3& v) { Position = v; updateTransformation(); }
	void setRotation(const vec3& v) { Rotation = v; updateTransformation(); }
	void setTexture(uint tex) { Texture = tex; }

	virtual void render();
	virtual void animate(float dt);

	void transform();
	void updateTransformation();
	mat4& getTransformationPointer() { return Transformation; }

private:
	vec3 Position;
	vec3 Rotation;
	mat4 Transformation;
	uint Texture;

	CObject* Parent;
	ObjectList Children;
};

#endif