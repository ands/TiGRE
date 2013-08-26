#pragma once

#include "common.h"

namespace TiGRE
{
	class ResourceException: public std::exception
	{
	public:
		ResourceException() { }
		ResourceException(std::string what) throw() : _what("[ResourceException] " + what) { }
		virtual ~ResourceException() throw() { }
	
		virtual const char* what() const throw()
		{
			return _what.c_str();
		}
	private:
		std::string _what;
	};

	class ResourceLoadException: public ResourceException
	{
	public:
		ResourceLoadException(std::string what) throw() : _what("[ResourceLoadException] " + what) { }
		virtual ~ResourceLoadException() throw() { }
	
		virtual const char* what() const throw()
		{
			return _what.c_str();
		}
	private:
		std::string _what;
	};

	class Resources;

	class Resource
	{
	public:
		Resources* resources() const;
		const std::string type() const;
		const std::string name() const;

	protected:
		friend Resources;
		Resource(Resources* resources, std::string type, std::string name);
		virtual ~Resource();

		virtual void loadFromFile(std::string filepath);
		virtual void loadFromXml(pugi::xml_node node);

	private:
		Resources* _resources;
		std::string _type;
		std::string _name;
	};
}
