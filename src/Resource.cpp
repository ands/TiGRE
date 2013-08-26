#include "Resource.h"
#include "Resources.h"

namespace TiGRE
{
	Resource::Resource(Resources* resources, std::string type, std::string name)
	{
		_resources = resources;
		_type = type;
		_name = name;
	}

	Resource::~Resource()
	{

	}

	Resources* Resource::resources() const
	{
		return _resources;
	}

	const std::string Resource::type() const
	{
		return _type;
	}
	
	const std::string Resource::name() const
	{
		return _name;
	}

	void Resource::loadFromFile(std::string /*filepath*/)
	{
		throw new ResourceLoadException(type() + "::loadFromFile(std::string filepath) not implemented");
	}

	void Resource::loadFromXml(pugi::xml_node /*node*/)
	{
		//throw new ResourceLoadException(type() + "::loadFromXml(pugi::xml_node node) not implemented");
	}
}
