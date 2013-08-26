#pragma once

#include "common.h"
#include "Object.h"

namespace TiGRE
{
	class Sound : public Object
	{
	public:
		void setState(bool on);
		bool isEnabled();
		void setLooping(bool on);
		void setPitch(float pitch);
		void setGain(float gain);
		
	protected:
		friend Resources;
		Sound(Resources* resources, std::string name);
		virtual ~Sound();

		virtual void updateTransform();

		virtual void loadFromFile(std::string filepath);
		virtual void loadFromWav(std::string filepath);
		virtual void loadFromXml(pugi::xml_node node);

	private:
		unsigned int alSource;
		unsigned int alSampleSet;
	};
}
