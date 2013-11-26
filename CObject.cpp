#include "CObject.h"

void CObject::setParent(CObject* parent)
{
	if (Parent)
		Parent->Children.remove(this);
	Parent = parent;
	if (Parent)
		Parent->Children.push_back(this);
}