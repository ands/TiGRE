#include "Mesh.h"
#include "Resources.h"

namespace TiGRE
{
	Mesh::Mesh(Resources* resources, std::string name) : Resource(resources, "Mesh", name)
	{
		vertexBufferObject = 0;
		indexBufferObject = 0;
	}

	Mesh::~Mesh()
	{
		if(vertexBufferObject != 0)
		{
			//glDeleteBuffers(1, &vertexBufferObject);
			vertexBufferObject = 0;
		}
		if(indexBufferObject != 0)
		{
			//glDeleteBuffers(1, &indexBufferObject);
			indexBufferObject = 0;
		}
	}

	void Mesh::loadFromFile(std::string filepath)
	{
		//we do only accept Wavefront *.obj files at the moment
		loadFromObj(filepath);
	}
	
	void Mesh::loadFromObj(std::string filepath)
	{
		std::vector<glm::vec3> points;
		std::vector<glm::vec3> pointNormals;
		std::vector<glm::vec2> pointTexcoords;
		std::vector<uint16_t> triangles;
		std::vector<uint16_t> quads;

		std::string  mtllib;
		std::string  usemtl;
	
		std::ifstream stream;
		stream.open(filepath.c_str(), std::fstream::in);
		if(!stream.is_open())
			throw new ResourceLoadException("Could not open mesh file \"" + filepath + "\"");
		else
		{
			std::string prefix;
			std::string name;
			float x, y, z;
		
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> texcoords;
			while(!stream.fail() && !stream.eof())
			{
				stream >> prefix;

				if(prefix == "mtllib")
				{
					stream >> mtllib;
					continue;
				}

				if(prefix == "usemtl")
				{
					if(triangles.size() > 0)
					{
						size_t trianglesSize = triangles.size() * sizeof(uint16_t);
						int trianglesOffset = resources()->geometryIndexData.add(&triangles[0], trianglesSize) / sizeof(GLushort);
						//TODO materials
						Surface surface = 
						{
							/*.material =*/ &Material::none,
							/*.primitive =*/ GL_TRIANGLES,
							/*.count =*/ (GLsizei)triangles.size(),
							/*.offset =*/ trianglesOffset,
							/*.indices =*/ triangles
						};
						if(usemtl.size() > 0)
							surface.material = resources()->getFromFile<Material>(mtllib + ";" + usemtl);
						surfaces.push_back(surface);
						triangles.clear();
					}

					if(quads.size() > 0)
					{
						size_t quadsSize = quads.size() * sizeof(uint16_t);
						int quadsOffset = resources()->geometryIndexData.add(&quads[0], quadsSize) / sizeof(GLushort);
						//TODO materials
						Surface surface = 
						{
							/*.material =*/ &Material::none,
							/*.primitive =*/ GL_QUADS,
							/*.count =*/ (GLsizei)quads.size(),
							/*.offset =*/ quadsOffset,
							/*.indices =*/ quads
						};
						if(usemtl.size() > 0)
							surface.material = resources()->getFromFile<Material>(mtllib + ";" + usemtl);
						surfaces.push_back(surface);
						quads.clear();
					}

					stream >> usemtl;
					continue;
				}
			
				switch (prefix[0])
				{
					case 'v':
					{
						if(prefix.size() == 1)
						{
							stream >> x >> y >> z;
							//std::cout << "point found: " << x << " " << y << " " << z << std::endl;
							glm::vec3 p(x, y, z);
							min = glm::min(p, min);
							max = glm::max(p, max);
							points.push_back(p);
							continue;
						}

						switch (prefix[1])
						{
							case 't': // texcoords
							{
								stream >> x >> y;
								//std::cout << "texcoord found: " << x << " " << y << std::endl;
								texcoords.push_back(glm::vec2(x, 1.0f - y));
							} break;
							case 'n': // normals
							{
								stream >> x >> y >> z;
								//std::cout << "normal found: " << x << " " << y << " " << z << std::endl;
								normals.push_back(glm::vec3(x, y, z));
							} break;
							default: // unknown
							{

							}
						}
					} break;
					case 'f': // faces (only triangles allowed)
					{
						//std::cout << "face found: ";
						uint16_t face[4];
						int i;
						for(i = 0; i < 4; i++)
						{
							char dummy;
							
							// vertex index
							stream >> face[i];
							face[i]--;
							//std::cout << pointIndex << " ";
							
							if(stream.peek() == '/')
							{
								stream.get(dummy);
								if(stream.peek() != '/')
								{
									// texcoord index
									uint16_t texcoordIndex;
									stream >> texcoordIndex;
									texcoordIndex--;
									glm::vec2 texcoord = texcoords[texcoordIndex];
									if(pointTexcoords.size() <= face[i])
										pointTexcoords.resize(face[i] + 1, texcoord);
									else
										pointTexcoords[face[i]] = texcoord;
								}
							}
							
							if(stream.peek() == '/')
							{
								stream.get(dummy);
								// normal index
								uint16_t normalIndex;
								stream >> normalIndex;
								normalIndex--;
								glm::vec3 normal = normals[normalIndex];
								if(pointNormals.size() <= face[i])
									pointNormals.resize(face[i] + 1, normal);
								else
									pointNormals[face[i]] = normal;
							}

							if(stream.peek() == '\n')
								break;
						}
						if(i < 2 /*|| i > 3*/)
							throw new ResourceLoadException("Unsupported face index count in mesh " + filepath);
						if(i == 2)
						{
							triangles.push_back(face[0]);
							triangles.push_back(face[1]);
							triangles.push_back(face[2]);
						}
						else if(i == 3)
						{
							quads.push_back(face[0]);
							quads.push_back(face[1]);
							quads.push_back(face[2]);
							quads.push_back(face[3]);
						}
						//std::cout << std::endl;
					} break;
					case 'g': // submesh
					case 'o':
					{
						stream >> name;
						//std::cout << "parsing submesh \"" << name << "\"" << std::endl;
					} break;
					default:
					{
						char line[256];
						stream.getline(line, 255);
						//std::cout << "Can't parse: " << line << std::endl;
					} // skip
				}
			}
			
			center = (min + max) * 0.5f;
			
			std::cout << "[loadSimpleWavefrontObj] \"" << filepath << "\""
				<< " points: " << points.size() 
				<< " normals: " << normals.size() 
				<< " texcoords: " << texcoords.size() 
				<< " triangles: " << (triangles.size() / 3)
				<< " quads: " << (quads.size() / 4) << std::endl;
		}
		stream.close();
		
		size_t pointsSize = points.size() * sizeof(glm::vec3);
		size_t normalsSize = pointNormals.size() * sizeof(glm::vec3);
		size_t texcoordsSize = pointTexcoords.size() * sizeof(glm::vec2);
		size_t trianglesSize = triangles.size() * sizeof(uint16_t);
		size_t quadsSize = quads.size() * sizeof(uint16_t);
		
		vertexSize = 3;
		vertexStride = 0;
		vertexOffset = 0;
	
		this->normalsUsed = normalsSize > 0;
		normalStride = 0;
		normalOffset = pointsSize;
	
		this->texcoordsUsed = texcoordsSize > 0;
		texcoordSize = 2;
		texcoordStride = 0;
		texcoordOffset = pointsSize + normalsSize;
		
		//generate normals if they don't exist
		if(!this->normalsUsed)
		{
			pointNormals.resize(points.size(), glm::vec3());
			//calculate cross products
			for(unsigned int i = 0; i < triangles.size(); i+=3)
			{
				int f[] = { triangles[i], triangles[i + 1], triangles[i + 2] };
				glm::vec3 n = glm::cross(points[f[1]] - points[f[0]], points[f[2]] - points[f[0]]);
				pointNormals[f[0]] += n; pointNormals[f[1]] += n; pointNormals[f[2]] += n;
			}
			for(unsigned int i = 0; i < quads.size(); i+=4)
			{
				int f[] = { quads[i], quads[i + 1], quads[i + 2], quads[i + 3] };
				glm::vec3 n = glm::cross(points[f[1]] - points[f[0]], points[f[2]] - points[f[0]]);
				pointNormals[f[0]] += n; pointNormals[f[1]] += n; pointNormals[f[2]] += n; pointNormals[f[3]] += n;
			}
			//normalize
			for(unsigned int i = 0; i < pointNormals.size(); i++)
			{
				pointNormals[i] = glm::normalize(pointNormals[i]);
				//std::cout << "Normal: " << pointNormals[i].X << ", " << pointNormals[i].Y << ", " << pointNormals[i].Z << std::endl;
			}
			normalsSize = pointNormals.size() * sizeof(glm::vec3);
			this->normalsUsed = true;
		}

		vertexBufferObject = resources()->geometryVertexData.handle();
		indexBufferObject = resources()->geometryIndexData.handle();
		vertexOffset = resources()->geometryVertexData.add(&points[0], pointsSize);
		if(this->normalsUsed)
			normalOffset = resources()->geometryVertexData.add(&pointNormals[0], normalsSize);
		if(this->texcoordsUsed)
			texcoordOffset = resources()->geometryVertexData.add(&pointTexcoords[0], texcoordsSize);

		if(triangles.size() > 0)
		{
			int trianglesOffset = resources()->geometryIndexData.add(&triangles[0], trianglesSize) / sizeof(GLushort);
			//TODO materials
			Surface surface = 
			{
				/*.material =*/ &Material::none,
				/*.primitive =*/ GL_TRIANGLES,
				/*.count =*/ (GLsizei)triangles.size(),
				/*.offset =*/ trianglesOffset,
				/*.indices =*/ triangles
			};
			if(usemtl.size() > 0)
				surface.material = resources()->getFromFile<Material>(mtllib + ";" + usemtl);
			surfaces.push_back(surface);
		}

		if(quads.size() > 0)
		{
			int quadsOffset = resources()->geometryIndexData.add(&quads[0], quadsSize) / sizeof(GLushort);
			//TODO materials
			Surface surface = 
			{
				/*.material =*/ &Material::none,
				/*.primitive =*/ GL_QUADS,
				/*.count =*/ (GLsizei)quads.size(),
				/*.offset =*/ quadsOffset,
				/*.indices =*/ quads
			};
			if(usemtl.size() > 0)
				surface.material = resources()->getFromFile<Material>(mtllib + ";" + usemtl);
			surfaces.push_back(surface);
		}

		this->vertices = points;
		this->normals = pointNormals;
		this->texcoords = pointTexcoords;
	}

	void Mesh::loadFromXml(pugi::xml_node node)
	{
		foreach (pugi::xml_node, surface_node, node)
		{
			pugi::xml_attribute id_attribute = surface_node->attribute("id");
			unsigned int id;
			if(!id_attribute || (id = id_attribute.as_uint()) >= surfaces.size())
				throw new ResourceLoadException("Error loading Mesh " + name() + " from XML: It contains a surface with an invalid id at position: " + std::to_string((long long)node.offset_debug()));

			pugi::xml_node material_node = surface_node->child("Material");
			if(material_node)
				surfaces[id].material = resources()->getFromXml<Material>(material_node);
		}
	}
}
