#include "Collider.h"

void Collider::SetEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;
}

void Collider::SetPhysicsMaterial(std::shared_ptr<PhysicsMaterial> physicsMaterial)
{
    _physicsMaterial = physicsMaterial;
}

void Collider::SetLayer(Layer layer)
{
    _layer = layer;
}

void Collider::SetLayerMask(LayerMask layerMask)
{
    _layerMask = layerMask;
}

void Collider::SetPosition(glm::vec3 position)
{
    _position = position;
}

void Collider::SetRotation(glm::vec3 rotation)
{
    _rotation = rotation;
    Update();
}

void Collider::SetScale(glm::vec3 scale)
{
    _scale = scale;
    Update();
}

bool Collider::IsEnabled()
{
    return _isEnabled;
}

std::shared_ptr<PhysicsMaterial> Collider::GetPhysicsMaterial()
{
    return _physicsMaterial;
}

Layer Collider::GetLayer()
{
    return _layer;
}

LayerMask Collider::GetLayerMask()
{
    return _layerMask;
}

glm::vec3 Collider::GetPosition()
{
    return _position;
}

glm::vec3 Collider::GetRotation()
{
    return _rotation;
}

glm::vec3 Collider::GetScale()
{
    return _scale;
}

ColliderType Collider::GetColliderType()
{
    return _colliderType;
}

Collider::Collider()
{
    _isEnabled = true;
    _physicsMaterial = std::make_shared<PhysicsMaterial>(0.6f, 0.4f, Average, Average);
    _layer = Layers::Default;
    _layerMask = LayerMasks::All;
    _colliderType = Box;
    _rotation = glm::vec3(0.0f);
    _scale = glm::vec3(1.0f);

    _boxCenter = glm::vec3(0.0f);
    _rawHorizontal = glm::vec3(1.0f, 0.0f, 0.0f);
    _rawVertical = glm::vec3(0.0f, 1.0f, 0.0f);
    _rawDepth = glm::vec3(0.0f, 0.0f, 1.0f);
    UpdateBoxVertices();

    _radius = 1.0f;
    _sphereCenter = glm::vec3(0.0f);
}

Collider::Collider(ColliderType colliderType)
{
    _isEnabled = true;
    _physicsMaterial = nullptr;
    _layer = Layers::Default;
    _layerMask = LayerMasks::All;
    _colliderType = colliderType;
    _rotation = glm::vec3(0.0f);
    _scale = glm::vec3(1.0f);

    _boxCenter = glm::vec3(0.0f);
    _horizontal = glm::vec3(1.0f, 0.0f, 0.0f);
    _vertical = glm::vec3(0.0f, 1.0f, 0.0f);
    _depth = glm::vec3(0.0f, 0.0f, 1.0f);
    UpdateBoxVertices();

    _radius = 1.0f;
    _sphereCenter = glm::vec3(0.0f);
}

float Collider::PointToPlane(glm::vec3 point, glm::vec4 plane)
{
    return glm::dot(glm::vec3(plane), point) + plane.w;
}

std::shared_ptr<CollisionInfo> Collider::CheckCollision(Collider &other)
{
    // TODO: considering layer
    CollisionInfo info = CollisionInfo(false);
    switch (_colliderType)
    {
    case Box:
        switch (other.GetColliderType())
        {
        case Box:
            return BoxToBox(other);
            break;

        case Sphere:
            return BoxToSphere(other);
            break;

        default:
            break;
        }
        break;

    case Sphere:
        switch (other.GetColliderType())
        {
        case Box:
            return SphereToBox(other);
            break;

        case Sphere:
            return SphereToSphere(other);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    std::shared_ptr<CollisionInfo> miss = std::make_shared<CollisionInfo>(false);
    return miss;
}

// TODO: Need Improvement: More correctly
std::shared_ptr<CollisionInfo> Collider::BoxToBox(Collider &other)
{
    std::vector<glm::vec3> reversedVertices = std::vector<glm::vec3>();
    std::vector<glm::vec3> verticesInside = std::vector<glm::vec3>();
    std::vector<glm::vec3> verticesOutside = std::vector<glm::vec3>();

    glm::mat4 reverseRotation = glm::mat4(1.0f);
    reverseRotation = glm::rotate(reverseRotation, glm::radians(-other.GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    reverseRotation = glm::rotate(reverseRotation, glm::radians(-other.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    reverseRotation = glm::rotate(reverseRotation, glm::radians(-other.GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 reverseTransform = glm::translate(glm::mat4(1.0f), _position - other.GetPosition());

    glm::mat4 reverseScale = glm::scale(
        glm::mat4(1.0f), glm::vec3(1.0f / other.GetScale().x, 1.0f / other.GetScale().y, 1.0f / other.GetScale().z));

    glm::mat4 reverseModelMatrix = reverseTransform * reverseRotation * reverseScale;

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(other.GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(other.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(other.GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), other.GetPosition());

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), other.GetScale());

    glm::mat4 modelMatrix = scale * rotation * transform;

    for (auto vertice : _vertices)
    {
        glm::vec3 reversedVertice = reverseModelMatrix * glm::vec4(vertice, 1.0f);

        reversedVertices.push_back(reversedVertice);
    }

    float maxDistance = 0;
    int farestIndex = 0;

    for (auto vertice : reversedVertices)
    {
        if (vertice.x >= -1.0f && vertice.x <= 1.0f && vertice.y >= -1.0f && vertice.y <= 1.0f && vertice.z >= -1.0f &&
            vertice.z <= 1.0f)
        {
            verticesInside.push_back(vertice);
        }
        else
        {
            verticesOutside.push_back(vertice);
            float distance = glm::length(vertice);
            if (distance > maxDistance)
            {
                maxDistance = distance;
                farestIndex = verticesOutside.size() - 1;
            }
        }
    }

    if (verticesInside.size() == 0 || verticesOutside.size() == 0)
    {
        std::shared_ptr<CollisionInfo> miss = std::make_shared<CollisionInfo>(false);
        return miss;
    }

    glm::vec3 selfCollisionVertice = glm::vec3(0.0f);
    for (auto vertice : verticesInside)
    {
        selfCollisionVertice += vertice;
    }
    selfCollisionVertice /= verticesInside.size();
    glm::vec3 farestVertice = verticesOutside[farestIndex];
    glm::vec3 collisionNormal = glm::normalize(farestVertice - selfCollisionVertice);

    std::vector<glm::vec4> planes = std::vector<glm::vec4>();
    planes.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    planes.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
    planes.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    planes.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
    planes.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    planes.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));

    for (auto plane : planes)
    {
        glm::vec3 normal = glm::vec3(plane);
        if (abs(glm::dot(normal, collisionNormal)) < 0.01f)
        {
            continue;
        }
        glm::vec3 point = ToolFunctions::LinePlaneIntersection(selfCollisionVertice, collisionNormal, plane);
        if ((point.x <= selfCollisionVertice.x && point.x >= farestVertice.x ||
             point.x >= selfCollisionVertice.x && point.x <= farestVertice.x) &&
            (point.y <= selfCollisionVertice.y && point.y >= farestVertice.y ||
             point.y >= selfCollisionVertice.y && point.y <= farestVertice.y) &&
            (point.z <= selfCollisionVertice.z && point.z >= farestVertice.z ||
             point.z >= selfCollisionVertice.z && point.z <= farestVertice.z))
        {
            selfCollisionVertice = modelMatrix * glm::vec4(selfCollisionVertice, 1.0f);
            point = modelMatrix * glm::vec4(point, 1.0f);
            std::shared_ptr<CollisionInfo> collisionInfo =
                std::make_shared<CollisionInfo>(true, selfCollisionVertice, point);
            return collisionInfo;
        }
    }

    std::shared_ptr<CollisionInfo> miss = std::make_shared<CollisionInfo>(false);
    return miss;
}

std::shared_ptr<CollisionInfo> Collider::BoxToSphere(Collider &other)
{
    return nullptr;
}

std::shared_ptr<CollisionInfo> Collider::SphereToBox(Collider &other)
{
    return nullptr;
}

std::shared_ptr<CollisionInfo> Collider::SphereToSphere(Collider &other)
{
    return nullptr;
}

void Collider::Update()
{
    glm::mat4 trans = glm::mat4(1.0f);

    // Update according to collider type
    float maxScale = 0;
    switch (_colliderType)
    {
    case Box:
        glm::scale(trans, _scale);
        trans = glm::rotate(trans, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        _horizontal = trans * glm::vec4(_rawHorizontal, 1.0f);
        _vertical = trans * glm::vec4(_rawVertical, 1.0f);
        _depth = trans * glm::vec4(_rawDepth, 1.0f);
        UpdateBoxVertices();
        break;

    case Sphere:
        maxScale = _scale.x;
        if (_scale.y > maxScale)
        {
            maxScale = _scale.y;
        }
        if (_scale.z > maxScale)
        {
            maxScale = _scale.z;
        }
        _radius *= maxScale;
        break;

    default:
        break;
    }
}

#pragma region Box

void Collider::UpdateBoxVertices()
{
    _vertices.clear();
    _vertices.push_back(_boxCenter - _horizontal - _vertical - _depth);
    _vertices.push_back(_boxCenter + _horizontal - _vertical - _depth);
    _vertices.push_back(_boxCenter - _horizontal + _vertical - _depth);
    _vertices.push_back(_boxCenter + _horizontal + _vertical - _depth);
    _vertices.push_back(_boxCenter - _horizontal - _vertical + _depth);
    _vertices.push_back(_boxCenter + _horizontal - _vertical + _depth);
    _vertices.push_back(_boxCenter - _horizontal + _vertical + _depth);
    _vertices.push_back(_boxCenter + _horizontal + _vertical + _depth);
}

void Collider::SetBoxHalfSize(glm::vec3 size)
{
    _rawHorizontal = glm::normalize(_rawHorizontal) * size.x;
    _rawVertical = glm::normalize(_rawVertical) * size.y;
    _rawDepth = glm::normalize(_rawDepth) * size.z;
    Update();
}

void Collider::SetBoxCenter(glm::vec3 center)
{
    _boxCenter = center;
    UpdateBoxVertices();
}

const std::vector<glm::vec3> &Collider::GetBoxVertices() const
{
    return _vertices;
}

glm::vec3 Collider::GetBoxSize() const
{
    return glm::vec3(glm::length(_horizontal), glm::length(_vertical), glm::length(_depth));
}

glm::vec3 Collider::GetBoxCenter() const
{
    return _boxCenter;
}

#pragma endregion

#pragma region Sphere

void Collider::SetSphereRadius(float radius)
{
    _radius = radius;
}

void Collider::SetSphereCenter(glm::vec3 center)
{
    _sphereCenter = center;
}

float Collider::GetSphereRadius() const
{
    return _radius;
}

glm::vec3 Collider::GetSphereCenter() const
{
    return _sphereCenter;
}

#pragma endregion