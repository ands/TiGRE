#pragma once

#include "common.h"
#include "Object.h"

namespace TiGRE
{
	class Light : public Object
	{
	public:
		void setAmbientColor(glm::vec4 color);
		void setDiffuseColor(glm::vec4 color);
		void setSpecularColor(glm::vec4 color);
		void setConstantAttenuation(float value);
		void setLinearAttenuation(float value);
		void setQuadraticAttenuation(float value);
		
		void setState(bool on);
		bool isEnabled();
		
		virtual void onRender(RenderPass renderPass, const Camera* camera) const;
		
	protected:
		friend Resources;
		Light(Resources* resources, std::string name);
		virtual ~Light();

		virtual void loadFromXml(pugi::xml_node node);

	private:
		GLuint id;
	};
}
