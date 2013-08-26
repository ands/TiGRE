#include "Resources.h"
#include "Renderer.h"

namespace TiGRE
{
	Renderer::Renderer() //:
		//_transparentGeometryIndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
	{
		_depthPass = true;
	}

	Renderer::~Renderer()
	{
	}
	
	void Renderer::init()
	{
		glEnable(GL_DEPTH_TEST);
	
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		//glDisable(GL_COLOR_MATERIAL);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_MULTISAMPLE);
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

		//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); // diese Technik ist für allgemeingültiges Alphablending leider unbrauchbar
	}
	
	void Renderer::add(const Object* obj)
	{
		_view.push_back(obj);
	
		if(obj->mesh != NULL)
		{
			foreach_const(std::vector<Surface>, surface, obj->mesh->surfaces)
			{
				if(!surface->material->blending || surface->primitive != GL_TRIANGLES) //TODO
				{
					std::pair<MaterialObjectSurfaces::iterator, bool> ret;
					ret = _materialGeometry.insert(MaterialObjectSurfaces::value_type(surface->material, ObjectSurfaces()));
					ret.first->second.push_back(ObjectSurface(obj, &(*surface)));
				}
				else
				{
					for(int index = 0; index < surface->count; index += 3) // only triangles currently
					{
						_transparentGeometry.push_back(RenderPrimitive(obj, &(*surface), index));
						_transparentGeometryDepths.push_back(RenderPrimitiveDepth());
					}
				}
			}
		}
		
		foreach_const(std::list<Object*>, subobj, *obj->children())
		{
			add(*subobj);
		}
	}
	
	void Renderer::clear()
	{
		_view.clear();
		_materialGeometry.clear();
		_transparentGeometry.clear();
		_transparentGeometryDepths.clear();
		_transparentGeometryIndices.clear();
		//_transparentGeometryIndexBuffer.clear();
		_depthBatches.clear();
	}
	
	void Renderer::setDepthPass(bool state)
	{
		_depthPass = state;
	}
	
	bool Renderer::isDepthPassEnabled() const
	{
		return _depthPass;
	}

	static glm::vec3 sortOrigin;

	static float calculateDepth(const RenderPrimitive *rp)
	{
		const glm::mat4 *transform = rp->object->transform();
		const Mesh *m = rp->object->mesh;

		float z = 0.0f;
		glm::vec4 vertex(0.0f, 0.0f, 0.0f, 1.0f);

		for(int i = 0; i < 3; i++)
		{
			vertex.xyz = m->vertices[rp->surface->indices[rp->primitive + i]];
			z += glm::length2((*transform * vertex).xyz - sortOrigin);
		}

		//z /= n;
		return z;
	}

	/*static bool depthCompare(RenderPrimitiveDepth p0, RenderPrimitiveDepth p1)
	{
		return p0.z > p1.z;
	}*/

	static int depthCompare(const void* p0, const void* p1)
	{
		float d = ((RenderPrimitiveDepth*)p1)->z - ((RenderPrimitiveDepth*)p0)->z;
		return d > 0.0f ? 1 : d == 0.0f ? 0 : -1;
	}

	static int objectDepthCompare(const void* p0, const void* p1)
	{
		const Object* o0 = *(const Object**)p0;
		const Object* o1 = *(const Object**)p1;
		float z0 = glm::length2(o0->absolutePosition() - sortOrigin);
		float z1 = glm::length2(o1->absolutePosition() - sortOrigin);
		float d = z1 - z0;
		return d > 0.0f ? 1 : d == 0.0f ? 0 : -1;
	}
	
	void Renderer::render(const Resources *resources, const Camera *camera)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(*camera->projection())); // apply camera projection matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(*camera->inverseTransform())); // apply camera view matrix

		// sorth objects in the view by depth for correct (billboard) transparency
		sortOrigin = camera->absolutePosition();
		qsort((void*)&_view[0], _view.size(), sizeof(_view[0]), objectDepthCompare);

		foreach_const(std::vector<const Object*>, object, _view)
			(*object)->onRender(RENDERPASS_PRERENDERING, camera);
		
		// depth pass
		if(_depthPass)
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			renderDepthPass(resources);
			foreach_const(std::vector<const Object*>, object, _view)
				(*object)->onRender(RENDERPASS_DEPTH, camera);
			glDepthMask(GL_FALSE);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthFunc(GL_EQUAL);
		}
		
		// default color pass
		renderColorPass(resources);
		foreach_const(std::vector<const Object*>, object, _view)
			(*object)->onRender(RENDERPASS_COLOR, camera);

		if(_depthPass)
		{
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
		}
		
		// transparent pass
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		if(_transparentGeometry.size() > 0)
		{
			for(unsigned int i = 0; i < _transparentGeometry.size(); i++)
			{
				_transparentGeometryDepths[i].primitive = i;
				_transparentGeometryDepths[i].z = calculateDepth(&_transparentGeometry[i]);
			}
			//std::sort(_transparentGeometryDepths.begin(), _transparentGeometryDepths.end(), depthCompare);
			qsort((void*)&_transparentGeometryDepths[0], _transparentGeometryDepths.size(), sizeof(_transparentGeometryDepths[0]), depthCompare);
			_transparentGeometryIndices.clear();
			_depthBatches.clear();
			RenderDepthBatch depthBatch(NULL, NULL, 0);
			foreach_const(RenderPrimitiveDepths, renderPrimitiveDepth, _transparentGeometryDepths)
			{
				const RenderPrimitive *renderPrimitive = &_transparentGeometry[renderPrimitiveDepth->primitive];
				if(renderPrimitive->object != depthBatch.object ||
					renderPrimitive->surface->material != depthBatch.material)
				{
					if(depthBatch.object != NULL)
						_depthBatches.push_back(depthBatch);
					depthBatch = RenderDepthBatch(renderPrimitive->surface->material, renderPrimitive->object, _transparentGeometryIndices.size());
				}

				for(int i = 0; i < 3; i++)
					_transparentGeometryIndices.push_back(renderPrimitive->surface->indices[renderPrimitive->primitive + i]);
				depthBatch.count += 3;
			}
			_depthBatches.push_back(depthBatch);
			//_transparentGeometryIndexBuffer.clear();
			//_transparentGeometryIndexBuffer.add(&_transparentGeometryIndices[0], _transparentGeometryIndices.size() * sizeof(GLushort));
			//_transparentGeometryIndexBuffer.bind();
			renderTransparencyPass(resources);
		}
		foreach_const(std::vector<const Object*>, object, _view)
			(*object)->onRender(RENDERPASS_TRANSPARENCY, camera);

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		foreach_const(std::vector<const Object*>, object, _view)
			(*object)->onRender(RENDERPASS_POSTRENDERING, camera);
		Material::none.bind();
	}
	
	void Renderer::renderDepthPass(const Resources *resources) const
	{
		resources->geometryVertexData.bind();
		resources->geometryIndexData.bind();

		foreach_const(MaterialObjectSurfaces, materialObjectSurfaces, _materialGeometry)
		{
			// render all surfaces to the depth buffer
			foreach_const(ObjectSurfaces, objectSurface, materialObjectSurfaces->second)
			{
				const Object* object = objectSurface->first;
				const Surface* surface = objectSurface->second;
				const Mesh* mesh = object->mesh;
			
				glPushMatrix();
				glMultMatrixf(glm::value_ptr(*object->transform()));

				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(mesh->vertexSize, GL_FLOAT, mesh->vertexStride, (const GLbyte*)0 + mesh->vertexOffset);
				glDrawElements(surface->primitive, surface->count, GL_UNSIGNED_SHORT, (const GLushort*)0 + surface->offset);
				glDisableClientState(GL_VERTEX_ARRAY);
			
				glPopMatrix();
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void Renderer::renderColorPass(const Resources *resources) const
	{
		resources->geometryVertexData.bind();
		resources->geometryIndexData.bind();

		foreach_const(MaterialObjectSurfaces, materialObjectSurfaces, _materialGeometry)
		{
			// bind material
			materialObjectSurfaces->first->bind();
			
			// render all surfaces with the selected material
			foreach_const(ObjectSurfaces, objectSurface, materialObjectSurfaces->second)
			{
				const Object* object = objectSurface->first;
				const Surface* surface = objectSurface->second;
				const Mesh* mesh = object->mesh;
				
				glPushMatrix();
				glMultMatrixf(glm::value_ptr(*object->transform()));

				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(mesh->vertexSize, GL_FLOAT, mesh->vertexStride, (const GLbyte*)0 + mesh->vertexOffset);
				
				if(mesh->normalsUsed)
				{
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT, mesh->normalStride, (const GLbyte*)0 + mesh->normalOffset);
				}
				
				if(mesh->texcoordsUsed)
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(mesh->texcoordSize, GL_FLOAT, mesh->texcoordStride, (const GLbyte*)0 + mesh->texcoordOffset);
				}
				
				glDrawElements(surface->primitive, surface->count, GL_UNSIGNED_SHORT, (const GLushort*)0 + surface->offset);
				
				if(mesh->texcoordsUsed)
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				if(mesh->normalsUsed)
					glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				
				glPopMatrix();
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Renderer::renderTransparencyPass(const Resources *resources) const
	{
		resources->geometryVertexData.bind();

		foreach_const(std::vector<RenderDepthBatch>, depthBatch, _depthBatches)
		{
			// bind material
			depthBatch->material->bind();
			
			// render primitive
			Mesh *mesh = depthBatch->object->mesh;
				
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(*depthBatch->object->transform()));

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(mesh->vertexSize, GL_FLOAT, mesh->vertexStride, (const GLbyte*)0 + mesh->vertexOffset);
			
			if(mesh->normalsUsed)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, mesh->normalStride, (const GLbyte*)0 + mesh->normalOffset);
			}
				
			if(mesh->texcoordsUsed)
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(mesh->texcoordSize, GL_FLOAT, mesh->texcoordStride, (const GLbyte*)0 + mesh->texcoordOffset);
			}
			
			glDrawElements(GL_TRIANGLES, depthBatch->count, GL_UNSIGNED_SHORT, /*(const GLushort*)0*/ &_transparentGeometryIndices[0] + depthBatch->offset);

			if(mesh->texcoordsUsed)
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			if(mesh->normalsUsed)
				glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
				
			glPopMatrix();
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
