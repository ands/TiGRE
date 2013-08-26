#include "Resources.h"

namespace TiGRE
{
	Resources::Resources() :
		geometryVertexData(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
		geometryIndexData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		for(int id = 7; id >= 0; id--)
			freeLightIds.push_back(id);
		
		std::cout << "Created Resources instance" << std::endl;
	}

	Resources::~Resources()
	{
		clear();
	}

	void Resources::clear()
	{
		typedef std::map<std::string, Resource*> NameResourceMap;
		foreach(NameResourceMap, resource, resources)
			delete resource->second;
		resources.clear();

		geometryVertexData.clear();
		geometryIndexData.clear();
	}

	void Resources::unload(std::string name)
	{
		resources.erase(name);
	}
}
