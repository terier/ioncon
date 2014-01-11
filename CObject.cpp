#include "CObject.h"

CObject::CObject(CObject* parent, const vec3& pos, const vec3& rot) :
Parent(0), Position(pos), Rotation(rot)
{
	for (int i=0; i<10; i++)
		Textures[i] = 0;
	setParent(Parent);
	updateTransformation();
}

void CObject::setParent(CObject* parent)
{
	if (Parent)
		Parent->Children.remove(this);
	Parent = parent;
	if (Parent)
		Parent->Children.push_back(this);
}

CObject::~CObject()
{
	setParent(0);
	for (ObjectIter it = Children.begin(); it != Children.end(); it++)
		delete *it;
}

mat4 CObject::getAbsoluteTransformation() const
{
	if (Parent)
		return Transformation * Parent->getAbsoluteTransformation();
	return Transformation;
}

vec3 CObject::getAbsolutePosition() const
{
	mat4 tr = getAbsoluteTransformation();
	return vec3(tr.M[12], tr.M[13], tr.M[14]);
}

void CObject::render()
{
	for (ObjectIter it = Children.begin(); it != Children.end(); it++)
		(*it)->render();
}

void CObject::animate(float dt)
{
	if (dt > 0)
		for (ObjectIter it = Children.begin(); it != Children.end(); it++)
			(*it)->animate(dt);
}

void CObject::transform()
{
	glMultMatrixf(Transformation.M);
}

void CObject::updateTransformation()
{
	Transformation.setIdentity();
	(Transformation.setTranslate(Position) *= mat4().setRotate(Rotation)).transpose();
}