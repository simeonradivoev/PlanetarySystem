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
			canvas->GetCurrentEvent().AddEvent(CE_PRESS);
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

	canvas->BeginLayout(x + style.leftMargin,y + style.topMargin,width - style.leftMargin - style.rightMargin,height - style.topMargin - style.bottomMargin);
	nvgRestore(vg);

	if (e.mouseAction == GLFW_PRESS)
	{
		if (OverRect(rec))
		{
			GUI::m_activeWindowID = id;
			canvas->GetCurrentEvent().AddEvent(CE_PRESS);
		}
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

std::string GUI::TextField(Canvas* canvas,Rect rec, std::string text, Style style)
{
	int ID = canvas->GetControl();
	NVGcontext* vg = canvas->GetContext();
	Canvas::Layout l = canvas->GetCurrentLayout();
	float x = rec.x + l.x;
	float y = rec.y + l.y;
	float width = rec.width;
	float height = rec.height;

	NVGpaint bg;
	// Edit
	bg = nvgBoxGradient(vg, x + 1, y + 1 + 1.5f, width - 2, height - 2, 3, 4, nvgRGBA(255, 255, 255, 32), nvgRGBA(32, 32, 32, 32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, width - 2, height - 2, 4 - 1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 0.5f, y + 0.5f, width - 1, height - 1, 4 - 0.5f);
	if (canvas->GetActiveControl() == ID)
		nvgStrokeColor(vg, style.pressFillColor);
	else
		nvgStrokeColor(vg, style.normalFillColor);
	nvgStroke(vg);

	nvgFontSize(vg, style.fontSize);
	nvgFontFace(vg, style.font);
	nvgFillColor(vg, style.textColor);
	nvgTextAlign(vg, style.textAllignment);
	nvgText(vg, x + style.leftMargin, y + height * 0.5f, text.c_str(), NULL);

	if (OverRect(Rect(x,y,width,height)) && Input::GetCurrentEvent().mouseAction == GLFW_PRESS)
	{
		if (canvas->GetActiveControl() != ID)
			canvas->SetControlOffset(text.length());

		canvas->SetActiveControl(ID);
		canvas->GetCurrentEvent().AddEvent(CE_ACTIVE_CONTROL);
		canvas->GetCurrentEvent().AddEvent(CE_PRESS);
	}

	if (canvas->GetActiveControl() == ID)
	{
		Rect ControlLine = GetControlLine(canvas, Rect(x + style.leftMargin, y + style.topMargin, 1, height - style.topMargin - style.bottomMargin), text);
		nvgBeginPath(vg);
		nvgFillColor(vg, style.textColor);
		nvgRect(vg, ControlLine.x, ControlLine.y, ControlLine.width, ControlLine.height);
		nvgFill(vg);

		text = ManageTextEdit(canvas, text);
	}


	return text;
}
std::string GUI::TextField(Canvas* canvas, Rect rec, std::string text, const char *style){
	return TextField(canvas,rec,text,canvas->GetStyle(style));
}
std::string GUI::TextField(Canvas* canvas, Rect rec, std::string text){
	return TextField(canvas, rec, text, canvas->GetCurrentStyle());
}
void GUI::ManageControlOffset(Canvas* canvas,unsigned int length)
{
	if (canvas->GetControlOffset() > length)
	{
		canvas->SetControlOffset(length);
	}

	if (canvas->GetActiveControl() != NULL)
	{
		if (Input::GetCurrentEvent().keyAction == GLFW_PRESS || Input::GetCurrentEvent().keyAction == GLFW_REPEAT)
		{
			if (Input::GetCurrentEvent().key == GLFW_KEY_LEFT)
			{
				if (canvas->GetControlOffset() > 0)
				{
					canvas->DecreaseControlOffset();
				}
			}
			else if (Input::GetCurrentEvent().key == GLFW_KEY_RIGHT)
			{
				if (canvas->GetControlOffset() < length)
				{
					canvas->IncreaseControlOffset();
				}
			}
		}
	}
}
std::string GUI::ManageTextEdit(Canvas* canvas, std::string text)
{
	ManageControlOffset(canvas, text.length());

	if (Input::GetCurrentEvent().characterPress)
	{
		if (text.empty() || canvas->GetControlOffset() >= text.length())
		{
			text += Input::GetCurrentEvent().character;
		}
		else
		{
			text.insert(canvas->GetControlOffset(), 1, Input::GetCurrentEvent().character);
		}

		canvas->IncreaseControlOffset();
	}


	if (Input::GetCurrentEvent().keyAction == GLFW_PRESS || Input::GetCurrentEvent().keyAction == GLFW_REPEAT)
	{
		if (Input::GetCurrentEvent().key == GLFW_KEY_BACKSPACE)
		{
			if (!text.empty() && canvas->GetControlOffset() > 0)
			{
				text.erase(canvas->GetControlOffset() - 1, 1);
				canvas->DecreaseControlOffset();
			}
			
		}
		else if (Input::GetCurrentEvent().key == GLFW_KEY_ENTER || Input::GetCurrentEvent().key == GLFW_KEY_ESCAPE)
		{
			Input::TakeCurrentEvent();
			canvas->SetActiveControl(NULL);
		}
	}

	return text;
}
GUI::Rect GUI::GetControlLine(Canvas* canvas, Rect size, std::string text)
{
	int pos = canvas->GetControlOffset();
	Rect controlSize(size.x, size.y, size.width, size.height);

	if (pos > 0){
		float* substrSize = new float[4];
		float* strSize = new float[4];
		std::string substr = text.substr(pos, text.length() - pos);
		float horizontalSize = nvgTextBounds(canvas->GetContext(), 0, 0, substr.c_str(), 0, substrSize);
		float horizontalSizeFull = nvgTextBounds(canvas->GetContext(), 0, 0, text.c_str(), 0, strSize);
		controlSize.x = size.x + (horizontalSizeFull - horizontalSize);
		delete substrSize;
		delete strSize;
	}
	return controlSize;
}
double GUI::Slider(Canvas* canvas, Rect rec, double value, Style style)
{
	int ID = canvas->GetControl();
	NVGpaint bg, knob;
	NVGcontext* vg = canvas->GetContext();
	Canvas::Layout l = canvas->GetCurrentLayout();
	float x = rec.x + l.x;
	float y = rec.y + l.y;
	float width = rec.width;
	float height = rec.height;
	float cy = y + (int)(height*0.5f);
	float kr = (int)(height*0.25f);

	nvgSave(vg);

	// Slot
	bg = nvgBoxGradient(vg, x, cy - 2 + 1, width, 4, 2, 2, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 128));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, cy - 2, width, 4, 2);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	// Knob Shadow
	bg = nvgRadialGradient(vg, x + (int)(value*width), cy + 1, kr - 3, kr + 3, nvgRGBA(0, 0, 0, 64), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, x + (int)(value*width) - kr - 5, cy - kr - 5, kr * 2 + 5 + 5, kr * 2 + 5 + 5 + 3);
	nvgCircle(vg, x + (int)(value*width), cy, kr);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	if (OverRect(Rect(x, y, width + kr, height)) && Input::GetCurrentEvent().mouseAction == GLFW_PRESS)
	{
		canvas->SetActiveControl(ID);
		canvas->GetCurrentEvent().AddEvent(CE_ACTIVE_CONTROL);
		canvas->GetCurrentEvent().AddEvent(CE_PRESS);
	}
	else if (canvas->GetActiveControl() == ID && Input::GetCurrentEvent().mouseAction == GLFW_RELEASE)
	{
		canvas->SetActiveControl(NULL);
	}
	else if (canvas->GetActiveControl() == ID && Input::GetCurrentEvent().mouseAction == GLFW_DRAGG)
	{
		value = (Input::GetCurrentEvent().mousePos.x - x) / width;
		value = glm::clamp(value,0.0,1.0);
	}

	// Knob
	knob = nvgLinearGradient(vg, x, cy - kr, x, cy + kr, nvgRGBA(255, 255, 255, 16), nvgRGBA(0, 0, 0, 16));
	nvgBeginPath(vg);
	nvgCircle(vg, x + (int)(value*width), cy, kr - 1);
	nvgFillColor(vg, (canvas->GetActiveControl() == ID ? style.pressFillColor : style.normalFillColor));
	nvgFill(vg);
	nvgFillPaint(vg, knob);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgCircle(vg, x + (int)(value*width), cy, kr - 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 92));
	nvgStroke(vg);

	nvgRestore(vg);

	return value;
}
double GUI::Slider(Canvas* canvas, Rect rec, double value, const char *style)
{
	return Slider(canvas, rec, value, canvas->GetStyle(style));
}
double GUI::Slider(Canvas* canvas, Rect rec, double value)
{
	return Slider(canvas,rec,value,canvas->GetCurrentStyle());
}