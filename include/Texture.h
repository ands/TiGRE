#pragma once

#include "common.h"
#include "Resource.h"

namespace TiGRE
{
	class Texture : public Resource
	{
	public:
		void bind() const;
		GLenum target() const;
		GLuint handle() const;
		glm::ivec2 size() const;
		
		static Texture none;

	protected:
		friend Resources;
		Texture(Resources* resources, std::string name);
		Texture(Resources* resources, std::string name, GLenum target);
		Texture(Resources* resources, std::string name, GLenum target, GLenum handle);
		virtual ~Texture();

		virtual void loadFromFile(std::string filepath);
		//virtual void loadFromPpm(std::string filepath);
		virtual void loadFromPng(std::string filepath);
		
	private:
		GLenum _target;
		GLuint _handle;
		glm::ivec2 _size;
	};
}
