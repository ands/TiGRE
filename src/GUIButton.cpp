#include "GUI.h"
#include "GUIButton.h"
#include "Resources.h"

namespace TiGRE
{
	void nothing() {}

	GUIButton::GUIButton(GUI* gui) : GUIControl(gui)
	{
		texture = gui->resources()->getFromFile<Texture>("gui/button.png");
		textureMouseover = gui->resources()->getFromFile<Texture>("gui/button_mouseover.png");
		texturePressed = gui->resources()->getFromFile<Texture>("gui/button_pressed.png");

		clickSound = gui->resources()->getFromFile<Sound>("sounds/click.wav");
		clickSound->setLooping(false);
		clickSound->setState(false);

		background = new GUITileSurface(gui);
		background->texture(texture);
		controls.push_back(background);

		text = new GUILabel(gui);
		text->text = "Default Window Title";
		text->font = GLUT_BITMAP_HELVETICA_10;
		text->color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		text->horizontalAlignment = ALIGNMENT_CENTER;
		text->verticalAlignment = ALIGNMENT_CENTER;
		controls.push_back(text);

		size(glm::vec2(96.0f, 24.0f));

		onClickCallback = nothing;
		pressed = false;
	}

	GUIButton::~GUIButton()
	{
	}

	glm::vec2 GUIButton::size() const
	{
		return GUIControl::size();
	}

	void GUIButton::size(const glm::vec2& size)
	{
		GUIControl::size(size);
		background->size(size);
		text->size(size);
	}

	bool GUIButton::onMouseButton(glm::vec2 position, MouseButton button, bool down)
	{
		if(GUIControl::onMouseButton(position, button, down))
		{
			return true;
		}
		if(button == MOUSEBUTTON_LEFT)
		{
			if(down)
			{
				if(!pressed)
				{
					pressed = true;
					background->texture(texturePressed);
					clickSound->setState(true);
					return true;
				}
			}
			else
			{
				if(pressed)
				{
					background->texture(texture);
					pressed = false;
					onClickCallback();
					return true;
				}
			}
		}
		return false;
	}
}
