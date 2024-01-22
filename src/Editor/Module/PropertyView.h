#pragma once

#include "Components/Mesh.h"
#include "Components/Rigidbody.h"
#include "Components/Texture.h"
#include "EditorModule.h"

#include "Entity.h"

class Transform;
class Rigidbody;

class PropertyView : public EditorModule
{
public:
    PropertyView();

    virtual void Update() override;

    void ViewPropEntity(Entity entity);
    void ViewPropTransform(Entity entity);
    void ViewPropRigidbody(Entity entity);
    void ViewPropTexture(Entity entity);
    void ViewPropMesh(Entity entity);
    void ViewPropShader(Entity entity);
};
