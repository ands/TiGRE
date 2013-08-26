#include "Light.h"
#include "Resources.h"
#include "Renderer.h"

namespace TiGRE
{
    Light::Light(Resources* resources, std::string name) : Object(resources, "Light", name)
    {
        this->id = resources->freeLightIds.back();
        resources->freeLightIds.pop_back();
        glEnable(GL_LIGHT0 + id);
        setAmbientColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        setSpecularColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    Light::~Light()
    {
        glDisable(GL_LIGHT0 + id);
        resources()->freeLightIds.push_back(this->id);
    }

    void Light::setAmbientColor(glm::vec4 color)
    {
        glLightfv(GL_LIGHT0 + id, GL_AMBIENT, glm::value_ptr(color));
    }

    void Light::setDiffuseColor(glm::vec4 color)
    {
        glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, glm::value_ptr(color));
    }

    void Light::setSpecularColor(glm::vec4 color)
    {
        glLightfv(GL_LIGHT0 + id, GL_SPECULAR, glm::value_ptr(color));
    }

    void Light::setConstantAttenuation(float value)
    {
        glLightf(GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, value);
    }

    void Light::setLinearAttenuation(float value)
    {
        glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, value);
    }

    void Light::setQuadraticAttenuation(float value)
    {
        glLightf(GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, value);
    }

    void Light::setState(bool on)
    {
        if(on)
            glEnable(GL_LIGHT0 + id);
        else
            glDisable(GL_LIGHT0 + id);
    }

    bool Light::isEnabled()
    {
        GLboolean state;
        glGetBooleanv(GL_LIGHT0 + id, &state);
        return state? true : false;
    }

    void Light::onRender(RenderPass renderPass, const Camera* /*camera*/) const
    {
        if(renderPass == RENDERPASS_PRERENDERING)
        {
            glPushMatrix();
            glMultMatrixf(glm::value_ptr(*transform()));
            GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // we already transformed to the lights position
            glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
            glPopMatrix();
        }
    }

    void Light::loadFromXml(pugi::xml_node node)
    {
        pugi::xml_node ambient = node.child("AmbientColor");
        if(ambient)
            setAmbientColor(glm::vec4(
                ambient.attribute("r").as_float(),
                ambient.attribute("g").as_float(),
                ambient.attribute("b").as_float(),
                ambient.attribute("a").as_float()));

        pugi::xml_node diffuse = node.child("DiffuseColor");
        if(diffuse)
            setDiffuseColor(glm::vec4(
                diffuse.attribute("r").as_float(),
                diffuse.attribute("g").as_float(),
                diffuse.attribute("b").as_float(),
                diffuse.attribute("a").as_float()));

        pugi::xml_node specular = node.child("SpecularColor");
        if(specular)
            setSpecularColor(glm::vec4(
                specular.attribute("r").as_float(),
                specular.attribute("g").as_float(),
                specular.attribute("b").as_float(),
                specular.attribute("a").as_float()));

        pugi::xml_node constantAttenuation = node.child("ConstantAttenuation");
        if(constantAttenuation)
            setConstantAttenuation(constantAttenuation.attribute("value").as_float());

        pugi::xml_node linearAttenuation = node.child("LinearAttenuation");
        if(linearAttenuation)
            setLinearAttenuation(linearAttenuation.attribute("value").as_float());

        pugi::xml_node quadraticAttenuation = node.child("QuadraticAttenuation");
        if(quadraticAttenuation)
            setQuadraticAttenuation(quadraticAttenuation.attribute("value").as_float());

        pugi::xml_node state = node.child("State");
        if(state)
            setState(state.attribute("enabled").as_bool());

        Object::loadFromXml(node);
    }
}
