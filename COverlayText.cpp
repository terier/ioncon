#include "COverlayText.h"

COverlayText::COverlayText(const std::string& text, void* font, const vec3& position, bool is3d, const vec3& color) :
CObject(0, position), Text(text), Font(font), Is3D(is3d), Color(color)
{
}

COverlayText::~COverlayText()
{
}

void COverlayText::render()
{

}