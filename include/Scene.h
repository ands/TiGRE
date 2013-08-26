#pragma once

#include "common.h"
#include "Object.h"

namespace TiGRE
{
	class Scene : public Object
	{
	protected:
		friend Resources;
		Scene(Resources* resources, std::string name);
		virtual ~Scene();

		virtual void loadFromFile(std::string filepath);
	};
}
