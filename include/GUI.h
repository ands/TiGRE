#pragma once

#include "common.h"
#include "GUIControl.h"
#include "GUIWindow.h"
#include "GUISurface.h"
#include "GUITileSurface.h"
#include "GUILabel.h"
#include "GUIButton.h"

namespace TiGRE
{
	class GUI
	{
	public:
		GUI(Resources *resources);
		virtual ~GUI();

		void clear();
		void size(glm::vec2 size);

		std::list<GUIWindow*> windows;
		
		bool onMouseButton(glm::vec2 position, MouseButton button, bool down);
		bool onMouseMove(glm::vec2 position);
		bool onCharacter(char character);
		void onDraw() const;

		Resources* resources() const;

	private:
		Resources* _resources;
		glm::mat4 projection;
	};
}
