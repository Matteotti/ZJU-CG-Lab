#pragma once

#include "System.h"

class TranslateSystem : public System
{
public:
    virtual void Init() override;
    virtual void Update(float dt) override;
    virtual int GetPriority() const override;
};