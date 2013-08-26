#include "GUI.h"
#include "Resources.h"
#include "GUITileSurface.h"

namespace TiGRE
{
	GUITileSurface::GUITileSurface(GUI* gui) : GUIControl(gui)
	{
		Texture* texture = gui->resources()->getFromFile<Texture>("gui/testframe.png");
		//glm::vec2 textureSize(texture->size());
		for(int y = 0; y < 3; y++)
		{
			for(int x = 0; x < 3; x++)
			{
				tiles[y][x] = new GUISurface(gui);
				tiles[y][x]->texture = texture;
				tiles[y][x]->textureRect(glm::vec2(x / 3.0f, y / 3.0f), glm::vec2(1.0f / 3.0f, 1.0f / 3.0f));//glm::vec2(texstart[x], texstart[y]) / textureSize, glm::vec2(texsize[x], texsize[y]) / textureSize);
				controls.push_back(tiles[y][x]);
			}
		}

		size(glm::vec2(1.0f, 1.0f));
	}

	GUITileSurface::~GUITileSurface()
	{
	}

	const Texture* GUITileSurface::texture() const
	{
		return tiles[0][0]->texture;
	}

	void GUITileSurface::texture(const Texture* texture)
	{
		for(int y = 0; y < 3; y++)
		{
			for(int x = 0; x < 3; x++)
			{
				tiles[y][x]->texture = texture;
			}
		}
	}

	void GUITileSurface::size(const glm::vec2& size)
	{
		GUIControl::size(size);

		glm::vec2 tileSize = glm::vec2(tiles[0][0]->texture->size()) / 3.0f;

		tiles[2][0]->position(glm::vec2(0.0f, 0.0f));
		tiles[2][0]->size(tileSize);

		tiles[2][1]->position(glm::vec2(tileSize.x , 0.0f));
		tiles[2][1]->size(glm::vec2(size.x - 2.0f * tileSize.x, tileSize.y));

		tiles[2][2]->position(glm::vec2(size.x - tileSize.x, 0.0f));
		tiles[2][2]->size(tileSize);


		tiles[1][0]->position(glm::vec2(0.0f, tileSize.y));
		tiles[1][0]->size(glm::vec2(tileSize.x, size.y - 2.0f * tileSize.y));

		tiles[1][1]->position(tileSize);
		tiles[1][1]->size(glm::vec2(size.x - 2.0f * tileSize.x, size.y - 2.0f * tileSize.y));

		tiles[1][2]->position(glm::vec2(size.x - tileSize.x, tileSize.y));
		tiles[1][2]->size(glm::vec2(tileSize.x, size.y - 2.0f * tileSize.y));


		tiles[0][0]->position(glm::vec2(0.0f, size.y - tileSize.y));
		tiles[0][0]->size(tileSize);

		tiles[0][1]->position(glm::vec2(tileSize.x, size.y - tileSize.y));
		tiles[0][1]->size(glm::vec2(size.x - 2.0f * tileSize.x, tileSize.y));

		tiles[0][2]->position(glm::vec2(size.x - tileSize.x, size.y - tileSize.y));
		tiles[0][2]->size(tileSize);
	}
}
