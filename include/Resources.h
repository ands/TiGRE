#pragma once

#include "common.h"
#include "Buffer.h"
#include "Resource.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "Object.h"
#include "Billboard.h"
#include "Camera.h"
#include "Light.h"
#include "Sound.h"
#include "Scene.h"

namespace TiGRE
{
	class Resources
	{
	public:
		Resources();
		virtual ~Resources();

		void clear();

		template<class T>
		T* getLoaded(std::string name)
		{
			std::map<std::string, Resource*>::iterator it = resources.find(name);
			if(it != resources.end())
			{
				T* resource = dynamic_cast<T*>(it->second);
				if(resource == NULL)
					throw new ResourceLoadException("Resource \"" + name + "\" has an unexpected type: " + it->second->type());
				return resource;
			}
			throw new ResourceLoadException("Resource \"" + name + "\" was not loaded yet");
		}

		template<class T>
		T* get(std::string name)
		{
			try
			{
				return getLoaded<T>(name);
			}
			catch(std::exception*)
			{
				T* resource = new T(this, name);
				if(resource == NULL)
					throw new ResourceLoadException("Could not create Resource instance \"" + name + "\"");
				resources.insert(std::make_pair(name, resource));
				std::cout << "Loaded " + resource->type() + " \"" + name + "\"" << std::endl;
				return resource;
			}
		}

		template<class T>
		T* getFromFile(std::string filepath)
		{
			try
			{
				return getLoaded<T>(filepath);
			}
			catch(std::exception*)
			{
				T* resource = new T(this, filepath);
				if(resource == NULL)
					throw new ResourceLoadException("Could not create Resource instance \"" + filepath + "\"");
				try
				{
					resource->loadFromFile(filepath);
				}
				catch(std::exception* exception)
				{
					delete resource;
					throw exception;
				}
				resources.insert(std::make_pair(filepath, resource));
				std::cout << "Loaded " + resource->type() + " \"" + filepath + "\"" << std::endl;
				return resource;
			}
		}

		template<class T>
		T* getFromXml(pugi::xml_node node)
		{
			T* resource;
			pugi::xml_attribute name = node.attribute("name");
			pugi::xml_attribute file = node.attribute("file");
			if(file)
				resource = getFromFile<T>(file.value());
			else if(name)
				resource = get<T>(name.value());
			else
				throw new ResourceLoadException("Could not load Resource from XML: " + node.path() + " is missing a name/file attribute at position: " + std::to_string((long long)node.offset_debug()));
			resource->loadFromXml(node);
			return resource;
		}

		void unload(std::string name);

		Buffer geometryVertexData;
		Buffer geometryIndexData;

	private:
		std::map<std::string, Resource*> resources;

		friend Light;
		std::vector<GLuint> freeLightIds;
	};
}
