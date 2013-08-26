#include "GUISurface.h"

namespace TiGRE
{
	GUISurface::GUISurface(GUI* gui) : GUIControl(gui)
	{
		textureRect(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	}

	GUISurface::~GUISurface()
	{

	}

	glm::vec2 GUISurface::textureStart() const
	{
		return glm::vec2(texcoords[0], texcoords[1]);
	}

	glm::vec2 GUISurface::textureEnd() const
	{
		return glm::vec2(texcoords[4], texcoords[5]);
	}

	void GUISurface::textureRect(const glm::vec2& start, const glm::vec2& size)
	{
		texcoords[0] = start.x;
		texcoords[1] = start.y + size.y;

		texcoords[2] = start.x + size.x;
		texcoords[3] = start.y + size.y;

		texcoords[4] = start.x + size.x;
		texcoords[5] = start.y;

		texcoords[6] = start.x;
		texcoords[7] = start.y;
	}

	void GUISurface::onDraw(glm::vec2 absolutePosition) const
	{
		GUIControl::onDraw(absolutePosition);

		float vertices[8];
		vertices[0] = absolutePosition.x;
		vertices[1] = absolutePosition.y;
		vertices[2] = absolutePosition.x + _size.x;
		vertices[3] = absolutePosition.y;
		vertices[4] = absolutePosition.x + _size.x;
		vertices[5] = absolutePosition.y + _size.y;
		vertices[6] = absolutePosition.x;
		vertices[7] = absolutePosition.y + _size.y;

		texture->bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}
