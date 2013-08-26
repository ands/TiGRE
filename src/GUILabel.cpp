#include "GUILabel.h"

namespace TiGRE
{
	GUILabel::GUILabel(GUI* gui) : GUIControl(gui)
	{
		text = "GUILabel";
		font = GLUT_BITMAP_8_BY_13;
		size(textSize());
	}

	GUILabel::~GUILabel()
	{
	}

	glm::vec2 GUILabel::textSize() const
	{
		return drawnStringSize(text, font);
	}

	void GUILabel::onDraw(glm::vec2 absolutePosition) const
	{
		GUIControl::onDraw(absolutePosition);
		glm::vec2 textSize = this->textSize();
		if(textSize != _size)
		{
			switch(horizontalAlignment)
			{
			case ALIGNMENT_BEGIN:
				break;
			case ALIGNMENT_CENTER:
				absolutePosition.x += (_size.x - textSize.x) * 0.5f;
				break;
			case ALIGNMENT_END:
				absolutePosition.x += _size.x - textSize.x;
				break;
			}
			switch(verticalAlignment)
			{
			case ALIGNMENT_BEGIN:
				break;
			case ALIGNMENT_CENTER:
				absolutePosition.y += (_size.y - textSize.y) * 0.5f;
				break;
			case ALIGNMENT_END:
				absolutePosition.y += _size.y - textSize.y;
				break;
			}
		}
		drawString(absolutePosition, text, font, color);
	}
}
