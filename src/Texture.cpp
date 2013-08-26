#include "Texture.h"
#include "lodepng.h"

namespace TiGRE
{
    Texture::Texture(Resources* resources, std::string name) : Resource(resources, "Texture", name)
    {
        this->_target = GL_TEXTURE_2D;
        glGenTextures(1, &this->_handle);
    }

    Texture::Texture(Resources* resources, std::string name, GLenum target) : Resource(resources, "Texture", name)
    {
        this->_target = target;
        glGenTextures(1, &this->_handle);
    }

    Texture::Texture(Resources* resources, std::string name, GLenum target, GLuint handle) : Resource(resources, "Texture", name)
    {
        this->_target = target;
        this->_handle = handle;
    }

    Texture::~Texture()
    {
        if(this->_handle != 0)
            glDeleteTextures(1, &this->_handle);
    }

    void Texture::bind() const
    {
        glBindTexture(this->_target, this->_handle);
    }

    GLenum Texture::target() const
    {
        return this->_target;
    }

    GLuint Texture::handle() const
    {
        return this->_handle;
    }

    glm::ivec2 Texture::size() const
    {
        return _size;
    }

    Texture Texture::none(NULL, "Texture", GL_TEXTURE_2D, 0);

    void Texture::loadFromFile(std::string filepath)
    {
        size_t dot = filepath.find_last_of('.', filepath.length());
        if(dot == std::string::npos)
            throw new ResourceLoadException("No file extension found in \"" + filepath + "\"");
        std::string extension = filepath.substr(dot + 1);
        std::for_each(extension.begin(), extension.end(), (int(*)(int))std::tolower);

        /*if(extension == "ppm")
        {
            loadFromPpm(filepath);
            return;
        }*/
        if(extension == "png")
        {
            loadFromPng(filepath);
            return;
        }

        throw new ResourceLoadException("Unknown texture file extension in \"" + filepath + "\"");
    }

    /*void Texture::loadFromPpm(std::string filepath)
    {
        bind();

        // anisotropic filtering
        GLint maximumAnisotropy;
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnisotropy);

        glTexEnvi(this->_target, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //glTexParameteri(this->_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameterf(this->_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(this->_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(this->_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(this->_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(this->_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if(!loadPPM(filepath.c_str()))
            throw new ResourceLoadException("Could not load PPM image \"" + filepath + "\"");
        glGetTexLevelParameteriv(this->_target, 0, GL_TEXTURE_WIDTH, &_size.x);
        glGetTexLevelParameteriv(this->_target, 0, GL_TEXTURE_HEIGHT, &_size.y);
    }*/

    void Texture::loadFromPng(std::string filepath)
    {
        std::vector<unsigned char> image;
        unsigned error = lodepng::decode(image, (unsigned&)_size.x, (unsigned&)_size.y, filepath);
        if(error != 0)
        {
            std::stringstream ss;
            ss << "lodepng error " << error << ": " << lodepng_error_text(error);
            throw new ResourceLoadException(ss.str());
        }
        bind();

        // anisotropic filtering
        GLint maximumAnisotropy;
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnisotropy);

        glTexEnvi(this->_target, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        //glTexParameteri(this->_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameterf(this->_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(this->_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(this->_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(this->_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(this->_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexImage2D(this->_target, 0, 4, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
        gluBuild2DMipmaps(this->_target, 4, _size.x, _size.y, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    }
}
