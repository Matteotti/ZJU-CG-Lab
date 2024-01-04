#pragma once

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

    void ViewProp(Entity entity);
    void ViewProp(Transform &tfComp);
    void ViewProp(Rigidbody &rbComp);
    void ViewProp(Texture &rbComp);
};
