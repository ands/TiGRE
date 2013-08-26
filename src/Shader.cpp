#include "Shader.h"
#include "Resources.h"

namespace TiGRE
{
	std::map<std::string, GLenum> Shader::shaderTypes;

	Shader::Shader(Resources* resources, std::string name) : Resource(resources, "Shader", name)
	{
		if(shaderTypes.size() == 0)
		{
			shaderTypes.insert(std::make_pair("vert", GL_VERTEX_SHADER));
			shaderTypes.insert(std::make_pair("tcon", GL_TESS_CONTROL_SHADER));
			shaderTypes.insert(std::make_pair("teva", GL_TESS_EVALUATION_SHADER));
			shaderTypes.insert(std::make_pair("geom", GL_GEOMETRY_SHADER));
			shaderTypes.insert(std::make_pair("frag", GL_FRAGMENT_SHADER));
		}
		
		_shader = 0;
	}

	Shader::~Shader()
	{
		if(_shader != 0)
			glDeleteShader(_shader);
	}
	
	void Shader::compile(GLenum type, std::string code)
	{
		if(_shader != 0)
			throw new ShaderException("Tried to recompile a shader");
		
		const GLchar* source = code.c_str();
		GLint length = code.length();
		_shader = glCreateShader(type);
		glShaderSource(_shader, 1, &source, &length);
		glCompileShader(_shader);
		
		GLint status;
		glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			GLint logLength = 511;
			GLchar log[512] = {0};
			glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &logLength);
			glGetShaderInfoLog(_shader, logLength, NULL, log);
			glDeleteShader(_shader);
			_shader = 0;
			throw new ShaderException("Failed to compile shader. Error log: " + std::string(log));
		}
	}
	
	void Shader::loadFromFile(std::string filepath)
	{
		// extract shader type
		if(filepath.length() <= 5 || filepath[filepath.length() - 5] != '.')
			throw new ResourceLoadException("Extension of file \"" + filepath + "\" should be one of: vert, tcon, teva, geom, frag");
		
		std::map<std::string, GLenum>::iterator it = shaderTypes.find(filepath.substr(filepath.length() - 4, 4));
		if(it == shaderTypes.end())
			throw new ResourceLoadException("Unknown shader file extension of file \"" + filepath + "\". Should be one of: vert, tcon, teva, geom, frag");
		GLenum type = it->second;

		// read code
		std::ifstream stream;
		stream.open(filepath.c_str(), std::fstream::in);
		if(!stream.is_open())
			throw new ResourceLoadException("Couldn't open file \"" + filepath + "\"");

		stream.seekg(0, std::ios::end);
		std::string code;
		code.resize((unsigned int)stream.tellg());
		stream.seekg(0, std::ios::beg);
		stream.read(&code[0], code.size());
		stream.close();

		// compile
		compile(type, code);
	}

	
	ShaderProgram::ShaderProgram(Resources* resources, std::string name) : Resource(resources, "ShaderProgram", name)
	{
		_program = 0;
	}

	ShaderProgram::~ShaderProgram()
	{
		if(_program != 0)
			glDeleteProgram(_program);
	}
	
	void ShaderProgram::bind() const
	{
		glUseProgram(this->_program);
	}
	
	ShaderProgram ShaderProgram::none(NULL, "none");
	
	void ShaderProgram::link(std::vector<Shader*> shaders)
	{
		if(_program != 0)
			throw new ShaderException("Tried to relink a shaderprogram");
	
		_program = glCreateProgram();
		foreach(std::vector<Shader*>, shader, shaders)
			glAttachShader(_program, (*shader)->_shader);
		glLinkProgram(_program);
		
		GLint status;
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if (!status)
		{
			GLint logLength = 511;
			GLchar log[512] = {0};
			glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);
			glGetProgramInfoLog(_program, logLength, NULL, log);
			glDeleteProgram(_program);
			_program = 0;
			throw new ShaderException("Failed to link program. Error log: " + std::string(log));
		}
	}

	void ShaderProgram::loadFromFile(std::string filepath)
	{
		std::vector<Shader*> shaders;
		std::stringstream ss(filepath);
		std::string item;
		while (std::getline(ss, item, ','))
			shaders.push_back(resources()->getFromFile<Shader>(item));
		link(shaders);
	}
}
