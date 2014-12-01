#include "gui.h"
#include "canvas.h"

bool GUI::pnpoly(int nvert, glm::vec2 *vert, glm::vec2 test)
{
	int i, j = 0;
	bool c = false;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((vert[i].y>test.y) != (vert[j].y > test.y)) &&
			(test.y < (vert[j].x - vert[i].x) * (test.y - vert[i].y) / (vert[j].y - vert[i].y) + vert[i].x))
			c = !c;
	}
	return c;
}

bool GUI::pointInsideRect(Rect rect, glm::vec2 point)
{
	if (point.x < rect.xmin() || point.x > rect.xmax() || point.y < rect.ymin() || point.y > rect.ymax())
		return false;
	else
	{
		return true;
	}
}

bool GUI::OverRect(Rect r)
{
	return pointInsideRect(r, Input::GetCurrentEvent().mousePos);
}

glm::vec2 GUI::WorldToScreen(glm::dvec3 worldPos, Camera& camera)
{
	glm::dvec4 point = camera.GetViewProjection() * glm::dvec4(worldPos, 1);
	glm::dvec3 vPoint = glm::dvec3(point.x, point.y, point.z) / point.w;
	glm::vec2 screenPoint = glm::vec2();
	float screenWidth = Display::GetCurrentDisplay()->GetWidth();
	float screenHeight = Display::GetCurrentDisplay()->GetHeight();
	float screenWidthHalf = screenWidth / 2.0;
	float screenHeightHalf = screenHeight / 2.0;
	screenPoint.x = ((vPoint.x + 1) / 2.0) * screenWidth;
	screenPoint.y = ((1 - vPoint.y) / 2.0) * screenHeight;
	return screenPoint;
}

bool GUI::WorlPointOnScreen(glm::dvec3 worldPos, Camera& camera)
{
	return (camera.GetViewProjection() * glm::dvec4(worldPos, 1)).w > 0;
}

bool GUI::Button(Canvas* canvas, Rect rect, const char* name, Style style)
{
	NVGcontext* vg = canvas->GetContext();

	nvgBeginPath(vg);
	nvgRoundedRect(vg, rect.x, rect.y, rect.width, rect.height, 16);
	if (OverRect(rect))
	{
		if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_1))
		{
			nvgFillColor(vg, style.pressFillColor);
		}
		else
		{
			nvgFillColor(vg, style.overFillColor);
		}

	}
	else
	{
		nvgFillColor(vg, style.normalFillColor);
	}

	nvgFill(vg);
	nvgTextAlign(vg, style.textAllignment);
	nvgFontSize(vg, style.fontSize);
	nvgFontFace(vg, style.font);

	nvgFillColor(vg, style.textColor);
	nvgText(vg, rect.x + style.leftMargin + (rect.width / 2) - style.topMargin, rect.y + style.topMargin + (rect.height / 2) - style.bottomMargin, name, NULL);
	nvgClosePath(vg);

	nvgBeginPath(vg);

	NVGpaint icon = nvgImagePattern(vg, rect.x + style.leftMargin, rect.y + style.topMargin, rect.width - style.leftMargin - style.rightMargin, rect.height - style.topMargin - style.bottomMargin, 0, style.imageHandler, style.imageAlpha);
	nvgRect(vg, rect.x + style.leftMargin, rect.y + style.topMargin, rect.width - style.leftMargin - style.rightMargin, rect.height - style.topMargin - style.bottomMargin);
	nvgFillPaint(vg, icon);
	nvgFill(vg);
	nvgClosePath(vg);

	//nvgTextBox(canvas->GetContext(),rect.x,rect.y,16,name,NULL);

	if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1)
	{
		if (pointInsideRect(rect, Input::GetCurrentEvent().mousePos))
		{
			return true;
		}
	}

	return false;
}
bool GUI::Button(Canvas* canvas, Rect rect, const char *name, const char *style)
{
	return GUI::Button(canvas, rect, name, canvas->GetStyle(style));
}
bool GUI::Button(Canvas* canvas, Rect rect, const char *name)
{
	return GUI::Button(canvas, rect, name, canvas->GetCurrentStyle());
}

void GUI::Label(Canvas* canvas, Rect rec, const char *text, Style style)
{
	NVGcontext* vg = canvas->GetContext();

	nvgFontFace(vg, style.font);
	nvgTextAlign(vg, style.textAllignment);
	nvgFillColor(vg, style.textColor);
	nvgText(vg, rec.x + style.leftMargin, rec.y + style.rightMargin, text, 0);
}
void GUI::Label(Canvas* canvas, Rect rec, const char *text, const char *style)
{
	GUI::Label(canvas, rec, text, canvas->GetStyle(style));
}
void GUI::Label(Canvas* canvas, Rect rec, const char *text)
{
	GUI::Label(canvas, rec, text, canvas->GetCurrentStyle());
}