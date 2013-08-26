#pragma once

#include "common.h"

namespace TiGRE
{
	class GUI;

	typedef void* Font;

	enum MouseButton
	{
		MOUSEBUTTON_LEFT, MOUSEBUTTON_MIDDLE, MOUSEBUTTON_RIGHT
	};

	class GUIControl
	{
	public:
		GUIControl(GUI *gui);
		virtual ~GUIControl();

		void clear();

		virtual glm::vec2 position() const;
		virtual glm::vec2 size() const;
		virtual void position(const glm::vec2& position);
		virtual void size(const glm::vec2& size);

		virtual bool onMouseButton(glm::vec2 position, MouseButton button, bool down);
		virtual bool onMouseMove(glm::vec2 position);
		virtual bool onCharacter(char character);
		virtual void onDraw(glm::vec2 absolutePosition) const;

		std::vector<GUIControl*> controls;

	protected:
		GUI* _gui;
		glm::vec2 _position;
		glm::vec2 _size;

		void drawString(glm::vec2 position, std::string string, Font font, glm::vec4 color) const;
		glm::vec2 drawnStringSize(std::string string, Font font) const;
	};
}
