#include "GUI.h"
#include "GUIWindow.h"
#include "Resources.h"

namespace TiGRE
{
	GUIWindow::GUIWindow(GUI* gui) : GUIControl(gui)
	{
		background = new GUITileSurface(gui);
		background->texture(gui->resources()->getFromFile<Texture>("gui/window.png"));
		controls.push_back(background);

		title = new GUILabel(gui);
		title->text = "Default Window Title";
		title->font = GLUT_BITMAP_HELVETICA_12;
		title->color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		title->horizontalAlignment = ALIGNMENT_CENTER;
		title->verticalAlignment = ALIGNMENT_CENTER;
		controls.push_back(title);

		size(glm::vec2(100.0f, 100.0f));
	}

	GUIWindow::~GUIWindow()
	{
	}

	glm::vec2 GUIWindow::size() const
	{
		return GUIControl::size();
	}

	void GUIWindow::size(const glm::vec2& size)
	{
		GUIControl::size(size);

		background->size(size);

		glm::vec2 tileSize = glm::vec2(background->texture()->size()) / 3.0f;
		title->position(glm::vec2(0.0f, size.y - tileSize.y));
		title->size(glm::vec2(size.x, tileSize.y));
	}
}
