#pragma once

#include "System.h"

class TranslateSystem : public System
{
public:
    TranslateSystem();
    
    virtual void Update(float dt) override;
    virtual int GetPriority() const override;
};