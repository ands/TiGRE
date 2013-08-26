#pragma once

#include "common.h"
#include "Object.h"
#include "Mesh.h"
#include "RenderPass.h"

namespace TiGRE
{
	class Resources;

	typedef std::pair<const Object*, const Surface*> ObjectSurface;
	typedef std::vector<ObjectSurface> ObjectSurfaces;
	typedef std::unordered_map<const Material*, ObjectSurfaces> MaterialObjectSurfaces;

	struct RenderDepthBatch
	{
		RenderDepthBatch(const Material* material, const Object* object, GLsizei offset)
		{
			this->material = material;
			this->object = object;
			this->offset = offset;
			this->count = 0;
		}
		const Material *material;
		const Object *object;
		GLsizei offset;
		GLsizei count;
	};

	struct RenderPrimitiveDepth
	{
		int primitive;
		float z;
	};
	typedef std::vector<RenderPrimitiveDepth> RenderPrimitiveDepths;

	struct RenderPrimitive
	{
		RenderPrimitive(const Object* o, const Surface* s, int p)
		{
			object = o;
			surface = s;
			primitive = p;
		}

		RenderPrimitive(const RenderPrimitive& rp)
		{
			object = rp.object;
			surface = rp.surface;
			primitive = rp.primitive;
		}

		const Object* object;
		const Surface* surface;
		int primitive;
	};
	typedef std::vector<RenderPrimitive> RenderPrimitives;

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();
		
		void init();
		void add(const Object* obj);
		void clear();
		
		void setDepthPass(bool state);
		bool isDepthPassEnabled() const;
		
		void render(const Resources *resources, const Camera *camera);

	private:
		void renderDepthPass(const Resources *resources) const;
		void renderColorPass(const Resources *resources) const;
		void renderTransparencyPass(const Resources *resources) const;
		
		std::vector<const Object*> _view;
		MaterialObjectSurfaces _materialGeometry;
		RenderPrimitives _transparentGeometry;
		RenderPrimitiveDepths _transparentGeometryDepths;
		//Buffer _transparentGeometryIndexBuffer;
		std::vector<GLushort> _transparentGeometryIndices;
		std::vector<RenderDepthBatch> _depthBatches;
		bool _depthPass;
	};
}
