#include "Scene.h"
#include "Resources.h"

namespace TiGRE
{
	Scene::Scene(Resources* resources, std::string name) : Object(resources, "Scene", name)
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::loadFromFile(std::string filepath)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filepath.c_str());
		if(result.status != pugi::status_ok)
			throw new ResourceLoadException("Error while parsing \"" + filepath + "\": " + result.description());

		loadFromXml(doc);
	}
}
