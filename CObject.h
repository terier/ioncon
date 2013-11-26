#ifndef OBJECT_H_INCL
#define OBJECT_H_INCL

#include <list>
#include "CMesh.h"
#include "opengl.h"

class CObject;

typedef std::list<CObject*> ObjectList;
typedef ObjectList::iterator ObjectIter;

class CObject
{
public:
	CObject(CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3()) :
		Parent(0), Position(pos), Rotation(rot)
	{
		setParent(Parent);
	}

	virtual ~CObject()
	{
		for (ObjectIter it = Children.begin(); it != Children.end(); it++)
			delete *it;
	}

	CObject* getParent() const { return Parent; }
	vec3 getPosition() const { return Position; }
	vec3 getRotation() const { return Rotation; }
	void setParent(CObject* parent);
	void setPosition(const vec3& v) { Position = v; }
	void setRotation(const vec3& v) { Rotation = v; }

	virtual void render()
	{
		for (ObjectIter it = Children.begin(); it != Children.end(); it++)
			(*it)->render();
	}

	void transform()
	{
		glTranslatef(Position.X, Position.Y, Position.Z);
		glRotatef(Rotation.X, 1,0,0);
		glRotatef(Rotation.Y, 0,1,0);
		glRotatef(Rotation.Z, 0,0,1);
	}

	virtual void animate(float dt)
	{
		if (dt > 0)
			for (ObjectIter it = Children.begin(); it != Children.end(); it++)
				(*it)->animate(dt);
	}

private:
	vec3 Position;
	vec3 Rotation;

	CObject* Parent;
	ObjectList Children;
};

#endif