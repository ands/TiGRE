#include "Camera.h"
#include "Renderer.h"

namespace TiGRE
{
	Camera::Camera(Resources* resources, std::string name) : Object(resources, "Camera", name)
	{
		setProjection(45.0f, 1.0f, 0.1f, 1000.0f);
	}

	Camera::~Camera()
	{
		
	}

	void Camera::updateListener()
	{
		glm::vec4 to = _transform * glm::vec4(0, 0, -1, 0);
		glm::vec4 up = _transform * glm::vec4(0, 1,  0, 0);
		listenerOrientation[0] = to.x; listenerOrientation[1] = to.y; listenerOrientation[2] = to.z;
		listenerOrientation[3] = up.x; listenerOrientation[4] = up.y; listenerOrientation[5] = up.z;
	}
	
	void Camera::onTransformUpdate()
	{
		_inverseTransform = glm::inverse(*transform());
		updateListener();
	}
	
	void Camera::setProjection(float fovy, float aspect, float znear, float zfar)
	{
		_projection = glm::perspective(fovy, aspect, znear, zfar);
	}

	void Camera::lookAt(glm::vec3 target, glm::vec3 up)
	{
		_inverseTransform = glm::lookAt(position, target, up);
		_transform = glm::inverse(_inverseTransform);
		updateListener();
	}
	
	const glm::mat4* Camera::projection() const
	{
		return &_projection;
	}
	
	const glm::mat4* Camera::inverseTransform() const
	{
		return &_inverseTransform;
	}

	void Camera::onRender(RenderPass renderPass, const Camera* camera) const
	{
		if(renderPass == RENDERPASS_PRERENDERING && camera == this)
		{
			alListenerfv(AL_POSITION, glm::value_ptr(position));
			alListenerfv(AL_ORIENTATION, listenerOrientation);
		}
	}

	void Camera::loadFromXml(pugi::xml_node node)
	{
		pugi::xml_node projection = node.child("Projection");
		if(projection)
			setProjection(
				projection.attribute("fovy").as_float(),
				projection.attribute("aspect").as_float(),
				projection.attribute("znear").as_float(),
				projection.attribute("zfar").as_float());

		Object::loadFromXml(node);
	}
}
