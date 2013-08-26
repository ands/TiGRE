#include "Billboard.h"
#include "Resources.h"
#include "Renderer.h"

namespace TiGRE
{
	Billboard::Billboard(Resources* resources, std::string name) : Object(resources, "Billboard", name)
	{
		setDepthTesting(true);
		setFrames(glm::ivec2(1, 1));
		setAnimationLooping(true);
		setAnimationSpeed(0.0);
		setAnimationPosition(0.0);
	}

	Billboard::~Billboard()
	{
	}

	void Billboard::setDepthTesting(bool depthTesting)
	{
		_depthTesting = depthTesting;
	}

	void Billboard::setFrames(const glm::ivec2& frames)
	{
		_frames = frames;
		_totalFrames = frames.x * frames.y;
	}

	void Billboard::setCurrentFrame(int frame)
	{
		setCurrentFrame(glm::ivec2(frame % _frames.x, frame / _frames.x));
	}

	void Billboard::setCurrentFrame(const glm::ivec2& frame)
	{
		glm::vec2 size(1.0f / (float)_frames.x, 1.0f / (float)_frames.y);

		texcoords[0] = (float)frame.x * size.x;
		texcoords[1] = (float)frame.y * size.y;

		texcoords[2] = texcoords[0] + size.x;
		texcoords[3] = texcoords[1];

		texcoords[4] = texcoords[0] + size.x;
		texcoords[5] = texcoords[1] + size.y;

		texcoords[6] = texcoords[0];
		texcoords[7] = texcoords[1] + size.y;
	}

	void Billboard::setAnimationLooping(bool looping)
	{
		_animationLooping = looping;
	}

	void Billboard::setAnimationSpeed(double fps)
	{
		_animationFps = fps;
	}

	void Billboard::setAnimationPosition(double position)
	{
		position = std::fmod((float)position, (float)_totalFrames);
		_animationPosition = position;
		setCurrentFrame((int)position);
	}

	void Billboard::onUpdate(double deltaTime)
	{
		Object::onUpdate(deltaTime);

		int lastFrame = (int)_animationPosition;
		_animationPosition += deltaTime * _animationFps;
		if(_animationLooping)
			_animationPosition = std::fmod((float)_animationPosition, (float)_totalFrames);
		int currentFrame = (int)_animationPosition;

		if(lastFrame != currentFrame)
		{
			if((int)_animationPosition < _totalFrames)
				setCurrentFrame(currentFrame);
		}
	}

	void Billboard::onRender(RenderPass renderPass, const Camera* camera) const
	{
		if(renderPass == RENDERPASS_TRANSPARENCY)
		{
			static float vertices[] =
			{
				-0.5f, -0.5f, 0.0f, 
				0.5f, -0.5f, 0.0f,
				0.5f, 0.5f, 0.0f,  
				-0.5f, 0.5f, 0.0f
			};
			static float normals[] =
			{
				0.0f, 0.0f, 1.0f, 
				0.0f, 0.0f, 1.0f, 
				0.0f, 0.0f, 1.0f, 
				0.0f, 0.0f, 1.0f
			};

			// at least allow some special scaling cases...
			glm::vec3 absoluteScale = scale; 
			Object* current = _parent;
			while(current != NULL)
			{
				absoluteScale.x *= current->scale.x;
				absoluteScale.y *= current->scale.y;
				absoluteScale.z *= current->scale.z;
				current = current->parent();
			}

			glm::mat4 m = glm::inverse(glm::lookAt(absolutePosition(), camera->absolutePosition(), glm::vec3(0, 1, 0)));
			m = glm::scale(m, absoluteScale) * glm::toMat4(rotation);

			glPushMatrix();
			glMultMatrixf(glm::value_ptr(m));

			material->bind();

			if(!_depthTesting)
				glDisable(GL_DEPTH_TEST);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, normals);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
			glDrawArrays(GL_QUADS, 0, 4);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			if(!_depthTesting)
				glEnable(GL_DEPTH_TEST);

			glPopMatrix();
		}
	}

	void Billboard::loadFromXml(pugi::xml_node node)
	{
		pugi::xml_node depthTesting = node.child("DepthTesting");
		if(depthTesting)
			setDepthTesting(depthTesting.attribute("enabled").as_bool());

		pugi::xml_node frames = node.child("Frames");
		if(frames)
			setFrames(glm::ivec2(
				frames.attribute("x").as_int(),
				frames.attribute("y").as_int()));

		pugi::xml_node animationLooping = node.child("AnimationLooping");
		if(animationLooping)
			setAnimationLooping(animationLooping.attribute("enabled").as_bool());

		pugi::xml_node animationSpeed = node.child("AnimationSpeed");
		if(animationSpeed)
			setAnimationSpeed(animationSpeed.attribute("value").as_double());

		pugi::xml_node material = node.child("Material");
		if(material)
			this->material = resources()->getFromXml<Material>(material);

		setAnimationPosition(0.0);

		Object::loadFromXml(node);
	}
}
