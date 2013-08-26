#pragma once

#include "common.h"
#include "Texture.h"
#include "GUIControl.h"
#include "GUISurface.h"

namespace TiGRE
{
	class GUI;

	class GUITileSurface : public GUIControl
	{
	public:
		GUITileSurface(GUI *gui);
		virtual ~GUITileSurface();

		const Texture* texture() const;
		void texture(const Texture* texture);
		virtual void size(const glm::vec2& size);

		GUISurface *tiles[3][3];
	};
}
