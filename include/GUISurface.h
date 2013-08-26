#pragma once

#include "common.h"
#include "GUIControl.h"
#include "Texture.h"

namespace TiGRE
{
	class GUISurface : public GUIControl
	{
	public:
		GUISurface(GUI* gui);
		virtual ~GUISurface();

		const Texture *texture;
		virtual glm::vec2 textureStart() const;
		virtual glm::vec2 textureEnd() const;
		virtual void textureRect(const glm::vec2& start, const glm::vec2& size);

		virtual void onDraw(glm::vec2 absolutePosition) const;
		
	private:
		GLfloat texcoords[8];
	};
}
