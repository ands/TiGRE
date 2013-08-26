#pragma once

#include "common.h"
#include "GUIControl.h"
#include "GUILabel.h"
#include "GUITileSurface.h"

namespace TiGRE
{
	class Sound;

	class GUIButton : public GUIControl
	{
	public:
		GUIButton(GUI* gui);
		virtual ~GUIButton();

		virtual glm::vec2 size() const;
		virtual void size(const glm::vec2& size);

		virtual bool onMouseButton(glm::vec2 position, MouseButton button, bool down);

		GUILabel *text;
		GUITileSurface *background;
		const Texture *texture;
		const Texture *textureMouseover;
		const Texture *texturePressed;

		Sound *clickSound;

		std::function<void()> onClickCallback;
		
	private:
		bool pressed;
	};
}
