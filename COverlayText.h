#ifndef COVERLAY_TEXT_H_INCL
#define COVERLAY_TEXT_H_INCL

#include "CObject.h"
#include <string>

class COverlayText : public CObject
{
public:
	COverlayText(const std::string& text, void* font,
		const vec3& position, bool is3d = false, const vec3& color = vec3());
	virtual ~COverlayText();

	std::string getText() { return Text; }
	void* getFont() { return Font; }
	bool is3D() { return Is3D; }
	vec3 getColor() { return Color; }

	void setText(const std::string& text) { Text = text; }
	void setFont(void* font) { Font = font; }
	void set3D(bool is3d) { Is3D = is3d; }
	void setColor(const vec3& color) { Color = color; }

	virtual void render();

private:
	std::string Text;
	void* Font;
	bool Is3D;
	vec3 Color;
};

#endif