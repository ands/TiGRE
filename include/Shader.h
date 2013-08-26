#pragma once

#include "common.h"
#include "Resource.h"

namespace TiGRE
{
	class ShaderException: public std::exception
	{
	public:
		ShaderException(std::string what) throw() : _what("[ShaderException] " + what) { }
		virtual ~ShaderException() throw() { }
	
		virtual const char* what() const throw()
		{
			return _what.c_str();
		}
	private:
		std::string _what;
	};

	class ShaderProgram;

	class Shader : public Resource
	{
	public:
		void compile(GLenum type, std::string code);

	protected:
		friend Resources;
		friend ShaderProgram;
		Shader(Resources* resources, std::string name);
		virtual ~Shader();

		virtual void loadFromFile(std::string filepath);

	private:
		GLuint _shader;
		static std::map<std::string, GLenum> shaderTypes;
	};
	
	class ShaderProgram : public Resource
	{
	public:
		void link(std::vector<Shader*> shaders);
		void bind() const;
		
		static ShaderProgram none;
		
	protected:
		friend Resources;
		ShaderProgram(Resources* resources, std::string name);
		virtual ~ShaderProgram();

		virtual void loadFromFile(std::string filepath);

	private:
		GLuint _program;
	};
}
