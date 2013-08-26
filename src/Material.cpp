#include "Material.h"
#include "Resources.h"

namespace TiGRE
{
	Material::Material(Resources* resources, std::string name) : Resource(resources, "Material", name)
	{
		emissionColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		ambientColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		shininess = 96.0f;
		blending = false;
		shader = NULL;
	}

	Material::~Material()
	{
	
	}

	void Material::bind() const
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(emissionColor));
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambientColor));
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuseColor));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specularColor));
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
			
		if(textures.size() > 0)
		{
			int id = 0;
			foreach_const(std::vector<const Texture*>, texture, textures)
			{
				glClientActiveTexture(GL_TEXTURE0 + id);
				//glActiveTexture(GL_TEXTURE0 + id);
				glEnable(GL_TEXTURE_2D);
				(*texture)->bind();
				id++;
			}
		}
		else
		{
			glClientActiveTexture(GL_TEXTURE0);
			//glActiveTexture(GL_TEXTURE0);
			//Texture::none.bind();
			glDisable(GL_TEXTURE_2D);
		}
			
		if(shader != NULL)
		{
			shader->bind();
		}
		else
		{
			ShaderProgram::none.bind();
		}
	}
	
	Material Material::none(NULL, "none");

	void Material::loadFromFile(std::string filepath)
	{
		// split by ';'
		std::stringstream ss(filepath);
		std::vector<std::string> parts;
		std::string item;
		while (std::getline(ss, item, ';'))
			parts.push_back(item);

		// extract file extension
		size_t dot = filepath.find_last_of('.', parts[0].length());
		if(dot == std::string::npos)
			throw new ResourceLoadException("No file extension found in \"" + parts[0] + "\"");
		std::string extension = parts[0].substr(dot + 1);
		std::for_each(extension.begin(), extension.end(), (int(*)(int))std::tolower);

		if(extension == "mtl")
		{
			if(parts.size() != 2)
				throw new ResourceLoadException("No material name for *.mtl file \"" + parts[0] + "\" specified");
			loadFromMtl(parts[0], parts[1]);
			return;
		}

		throw new ResourceLoadException("Unknown material file extension in \"" + parts[0] + "\"");
	}

	void Material::loadFromMtl(std::string filepath, std::string materialname)
	{
		std::ifstream stream;
		stream.open(filepath.c_str(), std::fstream::in);
		if(!stream.is_open())
			throw new ResourceLoadException("Could not open material file \"" + filepath + "\"");

		std::string prefix;
		std::string name;

		while(!stream.fail() && !stream.eof())
		{
			stream >> prefix;
			if(prefix != "newmtl")
				continue;

			stream >> name;
			if(name != materialname)
				continue;

			while(!stream.fail() && !stream.eof())
			{
				stream >> prefix;

				if(prefix == "newmtl")
					break;

				if(prefix == "Ke")
				{
					stream >> emissionColor.r;
					stream >> emissionColor.g;
					stream >> emissionColor.b;
				}
				else if(prefix == "Ka")
				{
					stream >> ambientColor.r;
					stream >> ambientColor.g;
					stream >> ambientColor.b;
				}
				else if(prefix == "Kd")
				{
					stream >> diffuseColor.r;
					stream >> diffuseColor.g;
					stream >> diffuseColor.b;
				}
				else if(prefix == "Ks")
				{
					stream >> specularColor.r;
					stream >> specularColor.g;
					stream >> specularColor.b;
				}
				else if(prefix == "Ns")
				{
					stream >> shininess;
				}
				else if(prefix == "d" || prefix == "Tr")
				{
					float alpha;
					stream >> alpha;
					if(alpha < 1.0f)
						blending = true;
					ambientColor.a = diffuseColor.a = alpha;
				}
				else if(prefix == "illum")
				{
					int mode;
					stream >> mode;
					switch(mode)
					{
						case 0: // Color on and Ambient off
						{
							ambientColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
							specularColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
						} break;
						case 1: // Color on and Ambient on
						{
							specularColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
						} break;
						case 2: // Highlight on
						case 3: // Reflection on and Ray trace on
						{
						} break;
						case 4: // Transparency: Glass on, Reflection: Ray trace on
						{
							blending = true;
						} break;
						case 5: // Reflection: Fresnel on and Ray trace on
						{
						} break;
						case 6: // Transparency: Refrection on, Reflection: Fresnel off and Ray trace on
						case 7: // Transparency: Refrection on, Reflection: Fresnel on and Ray trace on
						{
							blending = true;
						} break;
						case 8: // Reflection on and Ray trace off
						{
						} break;
						case 9: // Transparency: Glass on, Reflection: Ray trace off
						{
							blending = true;
						} break;
						case 10: // Casts shadows onto invisible surfaces
						{
						} break;
					}
				}
				else if(prefix == "map_Ka" || prefix == "map_Kd")
				{
					std::string texture;
					stream >> texture;
					if(texture.size() > 0)
						textures.push_back(resources()->getFromFile<Texture>(texture));
				}
				// TODO: "map_Ke", "map_Ks", "map_Ns", "map_d", "map_bump", "bump", "disp", "decal", "refl"
			}

			break;
		}
		stream.close();

		if(name != materialname)
			throw new ResourceLoadException("Could not find material \"" + materialname + "\" in file \"" + filepath + "\"");
	}

	void Material::loadFromXml(pugi::xml_node node)
	{
		pugi::xml_node emission_node = node.child("EmissionColor");
		if(emission_node)
			emissionColor = glm::vec4(
				emission_node.attribute("r").as_float(),
				emission_node.attribute("g").as_float(),
				emission_node.attribute("b").as_float(),
				emission_node.attribute("a").as_float());

		pugi::xml_node ambient_node = node.child("AmbientColor");
		if(ambient_node)
			ambientColor = glm::vec4(
				ambient_node.attribute("r").as_float(),
				ambient_node.attribute("g").as_float(),
				ambient_node.attribute("b").as_float(),
				ambient_node.attribute("a").as_float());

		pugi::xml_node diffuse_node = node.child("DiffuseColor");
		if(diffuse_node)
			diffuseColor = glm::vec4(
				diffuse_node.attribute("r").as_float(),
				diffuse_node.attribute("g").as_float(),
				diffuse_node.attribute("b").as_float(),
				diffuse_node.attribute("a").as_float());

		pugi::xml_node specular_node = node.child("SpecularColor");
		if(specular_node)
			specularColor = glm::vec4(
				specular_node.attribute("r").as_float(),
				specular_node.attribute("g").as_float(),
				specular_node.attribute("b").as_float(),
				specular_node.attribute("a").as_float());

		pugi::xml_node shininess_node = node.child("Shininess");
		if(shininess_node)
			shininess = shininess_node.attribute("value").as_float();

		pugi::xml_node blending_node = node.child("Blending");
		if(blending_node)
			blending = blending_node.attribute("enabled").as_bool();

		pugi::xml_node textures_node = node.child("Textures");
		if(textures_node)
			foreach (pugi::xml_node, texture_node, textures_node)
				textures.push_back(resources()->getFromXml<Texture>(*texture_node));

		pugi::xml_node shader_node = node.child("ShaderProgram");
		if(shader_node)
			shader = resources()->getFromXml<ShaderProgram>(shader_node);
	}
}
