#pragma once

#include "common.h"
#include "Object.h"

namespace TiGRE
{
	class Camera : public Object
	{
	public:
		void setProjection(float fovy, float aspect, float znear, float zfar);
		void lookAt(glm::vec3 target, glm::vec3 up);

		const glm::mat4* projection() const;
		const glm::mat4* inverseTransform() const;
		
	protected:
		friend Resources;
		Camera(Resources* resources, std::string name);
		virtual ~Camera();

		virtual void onRender(RenderPass renderPass, const Camera* camera) const;

		virtual void loadFromXml(pugi::xml_node node);

		virtual void onTransformUpdate();

	private:
		void updateListener();

		glm::mat4 _projection;
		glm::mat4 _inverseTransform;
		ALfloat	listenerOrientation[6];
		// Frustum frustum
	};
}
