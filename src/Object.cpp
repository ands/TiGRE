#include "Object.h"
#include "Resources.h"

namespace TiGRE
{
    Object::Object(Resources* resources, std::string name) : Resource(resources, "Object", name)
    {
        _parent = NULL;
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        updateTransform();
        mesh = NULL;
    }

    Object::Object(Resources* resources, std::string type, std::string name) : Resource(resources, type, name)
    {
        _parent = NULL;
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        updateTransform();
        mesh = NULL;
    }

    Object::~Object()
    {
        foreach(std::list<Object*>, object, _children)
            (*object)->_parent = NULL;
        if(_parent != NULL)
            _parent->_children.remove(this);
    }

    void Object::add(Object* object)
    {
        if(object->_parent != NULL)
            throw new ResourceException(
                    "Tried to attach " + object->type() + " " + object->name() +
                    " to " + type() + " " + name() + ", but it is already attached to " +
                    object->_parent->type() + " " + object->_parent->name());
        object->_parent = this;
        _children.push_back(object);
    }

    void Object::remove(Object* object)
    {
        if(object->_parent != this)
            throw new ResourceException(
                    "Tried to detach " + object->type() + " " + object->name() +
                    " from " + type() + " " + name() + ", but it is not attached to it");
        _children.remove(object);
        object->_parent = NULL;
    }

    Object* Object::parent() const
    {
        return _parent;
    }

    const std::list<Object*>* Object::children() const
    {
        return &_children;
    }

    glm::vec3 Object::absolutePosition() const
    {
        return _transform[3].xyz;
    }

    glm::quat Object::absoluteRotation() const
    {
        return glm::quat(_transform);
    }

    float Object::speed() const
    {
        return glm::length2(velocity);
    }

    const glm::mat4* Object::transform() const
    {
        return &_transform;
    }

    void Object::updateTransform()
    {
        if(_parent != NULL)
            _transform = _parent->_transform;
        else
            _transform = glm::mat4();

        _transform = glm::scale(glm::translate(_transform , position), scale) * glm::toMat4(rotation);

        onTransformUpdate();

        foreach(std::list<Object*>, child, _children)
        {
            (*child)->updateTransform();
        }
    }

    void Object::update(double deltaTime)
    {
        foreach(std::list<Object*>, child, _children)
        {
            (*child)->update(deltaTime);
        }
        onUpdate(deltaTime);
    }

    void Object::onTransformUpdate()
    {

    }

    void Object::onUpdate(double deltaTime)
    {
        glm::vec3 inc = velocity * (float)deltaTime;
        if(inc.x != 0.0f || inc.y != 0.0f || inc.z != 0.0f)
        {
            position += velocity * (float)deltaTime;
            updateTransform();
        }
    }

    void Object::onRender(RenderPass /*renderPass*/, const Camera* /*camera*/) const
    {

    }

    void Object::loadFromXml(pugi::xml_node node)
    {
        pugi::xml_node mesh_node = node.child("Mesh");
        if(mesh_node)
            mesh = resources()->getFromXml<Mesh>(mesh_node);

        pugi::xml_node position_node = node.child("Position");
        if(position_node)
            position = glm::vec3(
                position_node.attribute("x").as_float(),
                position_node.attribute("y").as_float(),
                position_node.attribute("z").as_float());
        pugi::xml_node rotation_node = node.child("Rotation");
        if(rotation_node)
            rotation = glm::quat(glm::vec3(
                rotation_node.attribute("x").as_float(),
                rotation_node.attribute("y").as_float(),
                rotation_node.attribute("z").as_float()));
        pugi::xml_node scale_node = node.child("Scale");
        if(scale_node)
            scale = glm::vec3(
                scale_node.attribute("x").as_float(),
                scale_node.attribute("y").as_float(),
                scale_node.attribute("z").as_float());

        pugi::xml_node children_node = node.child("Children");
        if(children_node)
        {
            foreach (pugi::xml_node, child_node, children_node)
            {
                std::string object_type(child_node->name());
                if(object_type == "Object")
                    add(resources()->getFromXml<Object>(*child_node));
                else if(object_type == "Billboard")
                    add(resources()->getFromXml<Billboard>(*child_node));
                else if(object_type == "Camera")
                    add(resources()->getFromXml<Camera>(*child_node));
                else if(object_type == "Light")
                    add(resources()->getFromXml<Light>(*child_node));
                else if(object_type == "Sound")
                    add(resources()->getFromXml<Sound>(*child_node));
                else
                    throw new ResourceLoadException("Error: \"" + object_type + "\" is not a known Object type at position: " + std::to_string((long long)node.offset_debug()));
            }
        }

        if(position_node || rotation_node || scale_node)
            updateTransform();
    }
}
