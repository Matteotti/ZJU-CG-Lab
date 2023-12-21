#pragma once

#include "Platform/ConInterface.h"

#include <Windows.h>

class ConInterfaceWinImpl : public ConInterface
{
public:
    ConInterfaceWinImpl();
    ~ConInterfaceWinImpl();

    virtual void MoveCursor(int x, int y) override;
    virtual void ClearScreen() override;
    virtual void SetTextColor(int color) override;

private:
    HANDLE _handle;
};
