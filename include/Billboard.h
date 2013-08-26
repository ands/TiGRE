#pragma once

#include "common.h"
#include "Object.h"

namespace TiGRE
{
	class Billboard : public Object
	{
	public:
		const Material *material;

		virtual void setDepthTesting(bool depthTesting);
		virtual void setFrames(const glm::ivec2& frames);
		virtual void setCurrentFrame(int frame);
		virtual void setCurrentFrame(const glm::ivec2& frame);
		virtual void setAnimationLooping(bool looping);
		virtual void setAnimationSpeed(double fps);
		virtual void setAnimationPosition(double position);
		
	protected:
		friend Resources;
		Billboard(Resources* resources, std::string name);
		virtual ~Billboard();

		virtual void onUpdate(double deltaTime);
		virtual void onRender(RenderPass renderPass, const Camera* camera) const;

		glm::ivec2 _frames;
		int _totalFrames;

		virtual void loadFromXml(pugi::xml_node node);

	private:
		GLfloat texcoords[8];
		bool _depthTesting;
		bool _animationLooping;
		double _animationPosition;
		double _animationFps;
	};
}
