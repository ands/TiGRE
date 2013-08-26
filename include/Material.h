#pragma once

#include "common.h"
#include "Texture.h"
#include "Shader.h"

namespace TiGRE
{
	class Material : public Resource
	{
	public:
		glm::vec4 emissionColor;
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		float shininess;
		bool blending;
		std::vector<const Texture*> textures;
		ShaderProgram* shader;

		void bind() const;
		
		static Material none;

	protected:
		friend Resources;
		Material(Resources* resources, std::string name);
		virtual ~Material();

		virtual void loadFromFile(std::string filepath);
		virtual void loadFromMtl(std::string filepath, std::string materialname);
		virtual void loadFromXml(pugi::xml_node node);
	};
}
