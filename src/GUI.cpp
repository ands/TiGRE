#include "GUI.h"

namespace TiGRE
{
	GUI::GUI(Resources *resources)
	{
		_resources = resources;
		projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f);
	}

	GUI::~GUI()
	{
		clear();
	}

	void GUI::clear()
	{
		foreach(std::list<GUIWindow*>, window, windows)
		{
			delete *window;
		}
		windows.clear();
	}

	void GUI::size(glm::vec2 size)
	{
		projection = glm::ortho(0.0f, size.x, 0.0f, size.y);
	}

	bool GUI::onMouseButton(glm::vec2 position, MouseButton button, bool down)
	{
		foreach(std::list<GUIWindow*>, window, windows)
		{
			if((*window)->onMouseButton(position - (*window)->position(), button, down))
			{
				if(button == MOUSEBUTTON_LEFT && down) // push active window to front
				{
					GUIWindow *tmp = *window;
					windows.erase(window);
					windows.push_front(tmp);
				}
				return true;
			}
		}
		return false;
	}

	bool GUI::onMouseMove(glm::vec2 position)
	{
		foreach(std::list<GUIWindow*>, window, windows)
		{
			if((*window)->onMouseMove(position - (*window)->position()))
			{
				return true;
			}
		}
		return false;
	}

	bool GUI::onCharacter(char character)
	{
		foreach(std::list<GUIWindow*>, window, windows)
		{
			if((*window)->onCharacter(character))
			{
				return true;
			}
		}
		return false;
	}
	
	void GUI::onDraw() const
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(projection));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glUseProgram(0);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		foreachr_const(std::list<GUIWindow*>, window, windows)
		{
			(*window)->onDraw((*window)->position());
		}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LIGHTING);
	}

	Resources* GUI::resources() const
	{
		return _resources;
	}
}
