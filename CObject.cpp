#include "CObject.h"

CObject::CObject(CObject* parent, const vec3& pos, const vec3& rot) :
Parent(0), Position(pos), Rotation(rot), Texture(0)
{
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
	/*mat4 T;
	Transformation.setIdentity();
	T.setRotate(Rotation);
	Transformation *= T;
	T.setTranslate(Position);
	Transformation *= T;
	Transformation.transpose();*/
	Transformation.setIdentity();
	//(Transformation.setRotate(Rotation) *= mat4().setTranslate(Position)).transpose();
	(Transformation.setTranslate(Position) *= mat4().setRotate(Rotation)).transpose();
	//Transformation.setTranslate(Position).transpose() *= mat4().setRotate(Rotation).transpose();
	//Transformation.setRotate(Rotation).transpose() *= mat4().setTranslate(Position).transpose();
}