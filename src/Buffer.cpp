#include "Buffer.h"

namespace TiGRE
{
	Buffer::Buffer(GLenum type, GLenum mode)
	{
		_type = type;
		_mode = mode;
		_bufferObject = 0;
		_occupied = 0;
		_capacity = 4 * 1024 * 1024;

		glGenBuffers(1, &_bufferObject);
		glBindBuffer(_type, _bufferObject);
		glBufferData(_type, _capacity, 0, _mode);
		glBindBuffer(_type, 0);
	}

	Buffer::~Buffer()
	{
		if(_bufferObject != 0)
		{
			glBindBuffer(_type, 0);
			glDeleteBuffers(1, &_bufferObject);
			_bufferObject = 0;
		}
	}

	GLenum Buffer::type() const
	{
		return _type;
	}

	GLenum Buffer::mode() const
	{
		return _mode;
	}

	GLuint Buffer::handle() const
	{
		return _bufferObject;
	}

	GLsizei Buffer::size() const
	{
		return _occupied;
	}

	GLsizei Buffer::capacity() const
	{
		return _capacity;
	}

	void Buffer::bind() const
	{
		glBindBuffer(_type, _bufferObject);
	}

	void Buffer::resize(GLsizei size)
	{
		if(size == _capacity)
			return;

		// initialize new buffer
		GLuint resizedBufferObject = 0;
		glGenBuffers(1, &resizedBufferObject);
		glBindBuffer(GL_COPY_WRITE_BUFFER, resizedBufferObject);
		glBufferData(GL_COPY_WRITE_BUFFER, size, 0, _mode);

		// copy over old buffer contents
		glBindBuffer(GL_COPY_READ_BUFFER, _bufferObject);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _occupied < size ? _occupied : size);

		// unbind buffers
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);

		// delete old buffer
		glDeleteBuffers(1, &_bufferObject);

		// update variables
		_bufferObject = resizedBufferObject;
		_capacity = size;
		if(_occupied > _capacity)
			_occupied = _capacity;
	}

	void Buffer::reserve(GLsizei size)
	{
		if(size > _capacity)
			resize(size);
	}

	GLsizei Buffer::add(GLvoid *data, GLsizei length)
	{
		if(_occupied + length > _capacity)
			reserve(_occupied + length); // maybe reserve some more? e.g. double the _capacity?

		GLsizei offset = _occupied;
		glBindBuffer(_type, _bufferObject);
		glBufferSubData(_type, offset, length, data);
		glBindBuffer(_type, 0);
		_occupied += length;
		return offset;
	}

	void Buffer::clear()
	{
		_occupied = 0;
	}
}
