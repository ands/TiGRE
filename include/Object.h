#pragma once

#include "common.h"
#include "Resource.h"
#include "Mesh.h"
#include "RenderPass.h"

namespace TiGRE
{
	class Scene;
	class Camera;
	class Renderer;

	class Object : public Resource
	{
	public:
		void add(Object* object);
		void remove(Object* object);
		Object* parent() const;
		const std::list<Object*>* children() const;

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		glm::vec3 absolutePosition() const;
		glm::quat absoluteRotation() const;

		glm::vec3 velocity;

		float speed() const;
		
		const glm::mat4* transform() const;
		virtual void updateTransform();
		virtual void update(double deltaTime);
		
		//bounding box
		//glm::vec3 min;
		//glm::vec3 max;
		
		//TODO: component/behaviour model
		
		Mesh* mesh;
		
	protected:
		friend Resources;
		friend Renderer;
		Object(Resources* resources, std::string name);
		Object(Resources* resources, std::string type, std::string name);
		virtual ~Object();

		virtual void loadFromXml(pugi::xml_node node);

		virtual void onTransformUpdate();
		virtual void onUpdate(double deltaTime);
		virtual void onRender(RenderPass renderPass, const Camera* camera) const;
	
		Object* _parent;
		std::list<Object*> _children;
		glm::mat4 _transform;
	};
}
