#pragma once

#include "common.h"
#include "GUIControl.h"

namespace TiGRE
{
	enum Alignment
	{
		ALIGNMENT_BEGIN, ALIGNMENT_CENTER, ALIGNMENT_END
	};

	class GUILabel : public GUIControl
	{
	public:
		GUILabel(GUI* gui);
		virtual ~GUILabel();

		std::string text;
		Font font;
		glm::vec4 color;
		Alignment horizontalAlignment;
		Alignment verticalAlignment;

		glm::vec2 textSize() const;

		virtual void onDraw(glm::vec2 absolutePosition) const;
	};
}
