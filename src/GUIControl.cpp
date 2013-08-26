#include "GUIControl.h"

namespace TiGRE
{
	GUIControl::GUIControl(GUI* gui)
	{
		_gui = gui;
		size(glm::vec2(1.0f, 1.0f));
	}

	GUIControl::~GUIControl()
	{
		clear();
	}

	void GUIControl::clear()
	{
		foreach(std::vector<GUIControl*>, control, controls)
		{
			delete *control;
		}
		controls.clear();
	}

	glm::vec2 GUIControl::position() const
	{
		return _position;
	}

	glm::vec2 GUIControl::size() const
	{
		return _size;
	}

	void GUIControl::position(const glm::vec2& position)
	{
		_position = position;
	}

	void GUIControl::size(const glm::vec2& size)
	{
		_size = size;
	}

	bool GUIControl::onMouseButton(glm::vec2 position, MouseButton button, bool down)
	{
		foreach(std::vector<GUIControl*>, control, controls)
		{
			if(position.x >= (*control)->_position.x &&
				position.y >= (*control)->_position.y &&
				position.x < (*control)->_position.x + (*control)->_size.x &&
				position.y < (*control)->_position.y + (*control)->_size.y)
			{
				if((*control)->onMouseButton(position - (*control)->_position, button, down))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool GUIControl::onMouseMove(glm::vec2 position)
	{
		foreach(std::vector<GUIControl*>, control, controls)
		{
			if(position.x >= (*control)->_position.x &&
				position.y >= (*control)->_position.y &&
				position.x < (*control)->_position.x + (*control)->_size.x &&
				position.y < (*control)->_position.y + (*control)->_size.y)
			{
				if((*control)->onMouseMove(position - (*control)->_position))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool GUIControl::onCharacter(char character)
	{
		foreach(std::vector<GUIControl*>, control, controls)
		{
			if((*control)->onCharacter(character))
			{
				return true;
			}
		}
		return false;
	}

	void GUIControl::onDraw(glm::vec2 absolutePosition) const
	{
		foreach_const(std::vector<GUIControl*>, control, controls)
		{
			(*control)->onDraw(absolutePosition + (*control)->_position);
		}
	}

	void GUIControl::drawString(glm::vec2 position, std::string string, Font font = GLUT_BITMAP_8_BY_13, glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)) const
	{
		const unsigned char* str = (const unsigned char*)string.c_str();
		glDisable(GL_TEXTURE_2D);
		glColor4fv(glm::value_ptr(color));
		if(font == GLUT_STROKE_ROMAN || font == GLUT_STROKE_MONO_ROMAN)
		{
			glPushMatrix();
			glTranslatef(position.x, position.y, 0.0f); // TODO: upgrade this for newer opengl versions
			glutStrokeString(font, str);
			glPopMatrix();
		}
		else
		{
			glRasterPos2f(position.x, position.y);
			glutBitmapString(font, str);
		}
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
	}

	glm::vec2 GUIControl::drawnStringSize(std::string string, Font font = GLUT_BITMAP_8_BY_13) const
	{
		const unsigned char* str = (const unsigned char*)string.c_str();
		if(font == GLUT_STROKE_ROMAN || font == GLUT_STROKE_MONO_ROMAN)
		{
			return glm::vec2(glutStrokeLength(font, str), glutStrokeHeight(font));
		}
		else
		{
			return glm::vec2(glutBitmapLength(font, str), glutBitmapHeight(font) - 4.0f); // glutBitmapHeight is giving too bog values most of the time
		}
	}
}
