#pragma once

#include "common.h"

namespace TiGRE
{
	class Buffer
	{
	public:
		Buffer(GLenum type, GLenum mode);
		virtual ~Buffer();

		GLenum type() const;
		GLenum mode() const;
		GLuint handle() const;
		GLsizei size() const;
		GLsizei capacity() const;

		void bind() const;
		void resize(GLsizei size);
		void reserve(GLsizei size);
		GLsizei add(GLvoid *data, GLsizei length);
		void clear();

	protected:
		GLenum _type;
		GLenum _mode;
		GLuint _bufferObject;
		GLsizei _capacity;
		GLsizei _occupied;
	};
}
