#include "gui.h"
#include "canvas.h"
#include "shader.h"
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

int GUI::m_activeWindowID = -1;
bool GUI::m_windowDragg = false;

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

void GUI::DrawLine(glm::dvec3 from, glm::dvec3 to,glm::vec4 Color,Shader* shader)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->Bind();
	shader->Update(Transform(), *Camera::GetCurrentCamera());
	shader->SetColor(Color);

	glBegin(GL_LINE_STRIP);
	glVertex3dv(glm::value_ptr(from));
	glVertex3dv(glm::value_ptr(to));
	glEnd();
	glDisable(GL_BLEND);
}

bool GUI::Button(Canvas* canvas, Rect rect, const char* name, Style style)
{
	NVGcontext* vg = canvas->GetContext();
	Canvas::Layout l = canvas->GetCurrentLayout();
	float x = rect.x + l.x;
	float y = rect.y + l.y;

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, rect.width, rect.height, 16);

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
	nvgText(vg, x + style.leftMargin + (rect.width / 2) - style.topMargin, y + style.topMargin + (rect.height / 2) - style.bottomMargin, name, NULL);
	nvgClosePath(vg);

	nvgBeginPath(vg);

	NVGpaint icon = nvgImagePattern(vg, rect.x + style.leftMargin + l.x, rect.y + style.topMargin, rect.width - style.leftMargin - style.rightMargin, rect.height - style.topMargin - style.bottomMargin, 0, style.imageHandler, style.imageAlpha);
	nvgRect(vg, x + style.leftMargin, y + style.topMargin, rect.width - style.leftMargin - style.rightMargin, rect.height - style.topMargin - style.bottomMargin);
	nvgFillPaint(vg, icon);
	nvgFill(vg);
	nvgClosePath(vg);

	//nvgTextBox(canvas->GetContext(),rect.x,rect.y,16,name,NULL);

	if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1)
	{
		if (pointInsideRect(Rect(x,y,rect.width,rect.height), Input::GetCurrentEvent().mousePos))
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
	Canvas::Layout l = canvas->GetCurrentLayout();

	nvgFontFace(vg, style.font);
	nvgFontSize(vg, style.fontSize);
	nvgTextAlign(vg, style.textAllignment);
	nvgFillColor(vg, style.textColor);
	nvgText(vg, rec.x + l.x + style.leftMargin, rec.y + l.y + style.topMargin, text, 0);
}
void GUI::Label(Canvas* canvas, Rect rec, const char *text, const char *style)
{
	GUI::Label(canvas, rec, text, canvas->GetStyle(style));
}
void GUI::Label(Canvas* canvas, Rect rec, const char *text)
{
	GUI::Label(canvas, rec, text, canvas->GetCurrentStyle());
}

GUI::Rect GUI::BeginWindow(Canvas* canvas, int id, Rect rec, const char *title, Style style)
{
	NVGcontext* vg = canvas->GetContext();
	Canvas::Layout l = canvas->GetCurrentLayout();
	Input::InputEvent e = Input::GetCurrentEvent();
	float headerSize = 32;
	float x = rec.x + l.x;
	float y = rec.y + l.y + headerSize;
	float width = rec.width;
	float height = rec.height - headerSize;
	float cornerRadius = 3.0;
	
	NVGpaint shadowPaint;
	NVGpaint headerPaint;

	nvgSave(vg);

	//window
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, width, height, cornerRadius);
	if (GUI::m_activeWindowID == id)
		nvgFillColor(vg, style.pressFillColor);
	else
		nvgFillColor(vg, style.normalFillColor);
	nvgFill(vg);

	// Drop shadow
	shadowPaint = nvgBoxGradient(vg, x, y + 2, width, height, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, x - 10, y - 10, width + 20, height + 30);
	nvgRoundedRect(vg, x, y, width, height, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	// Header
	headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, width - 2, headerSize, cornerRadius - 1);
	nvgFillPaint(vg, headerPaint);
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
	nvgLineTo(vg, x + 0.5f + width - 1, y + 0.5f + 30);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
	nvgStroke(vg);

	canvas->BeginLayout(x + style.leftMargin,y + style.topMargin);
	nvgRestore(vg);

	if (e.mouseAction == GLFW_PRESS)
	{
		if (OverRect(rec))
			GUI::m_activeWindowID = id;
		if (OverRect(Rect(x,y,rec.width,headerSize)))
			m_windowDragg = true;
	}
	else if (e.mouseAction == GLFW_RELEASE && m_activeWindowID == id)
	{
		m_windowDragg = false;
	}

	if (e.mouseAction == GLFW_DRAGG && m_windowDragg && m_activeWindowID == id)
	{
		rec.x += Input::GetCurrentEvent().mouseDelta.x;
		rec.y += Input::GetCurrentEvent().mouseDelta.y;
	}

	return rec;
}
GUI::Rect GUI::BeginWindow(Canvas* canvas, int id, Rect rec, const char *title, const char *style)
{
	return GUI::BeginWindow(canvas,id, rec, title, canvas->GetStyle(style));
}
GUI::Rect GUI::BeginWindow(Canvas* canvas, int id, Rect rec, const char *title)
{
	return GUI::BeginWindow(canvas,id, rec, title, canvas->GetCurrentStyle());
}
void GUI::EndWindow(Canvas* canvas)
{
	canvas->EndLayout();
}