#pragma once

#include "common.h"
#include "Material.h"

namespace TiGRE
{
	struct Surface
	{
		Material* material;
		GLenum primitive;
		GLsizei count;
		GLsizei offset;

		std::vector<uint16_t> indices;
	};

	class Mesh : public Resource
	{
	public:
		glm::vec3 center;
		glm::vec3 min;
		glm::vec3 max;
		
		GLuint vertexBufferObject;
		GLuint indexBufferObject;
		
		GLint   vertexSize;
		GLsizei vertexStride;
		GLsizei vertexOffset;
		
		bool normalsUsed;
		GLsizei normalStride;
		GLsizei normalOffset;
		
		bool texcoordsUsed;
		GLint   texcoordSize;
		GLsizei texcoordStride;
		GLsizei texcoordOffset;
		
		std::vector<Surface> surfaces;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texcoords;

	protected:
		friend Resources;
		Mesh(Resources* resources, std::string name);
		virtual ~Mesh();

		virtual void loadFromFile(std::string filepath);
		virtual void loadFromObj(std::string filepath);
		virtual void loadFromXml(pugi::xml_node node);
	};
}
