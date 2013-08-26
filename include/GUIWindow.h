#pragma once

#include "common.h"
#include "GUIControl.h"
#include "GUILabel.h"
#include "GUITileSurface.h"

namespace TiGRE
{
	class GUI;

	class GUIWindow : public GUIControl
	{
	public:
		GUIWindow(GUI *gui);
		virtual ~GUIWindow();

		virtual glm::vec2 size() const;
		virtual void size(const glm::vec2& size);

		GUILabel *title;
		GUITileSurface *background;
	};
}
