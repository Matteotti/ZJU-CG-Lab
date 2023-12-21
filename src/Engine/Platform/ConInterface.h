#pragma once

class ConInterface
{
public:
    virtual void MoveCursor(int x, int y) = 0;
    virtual void ClearScreen() = 0;
    virtual void SetTextColor(int color) = 0;

    static ConInterface *getConInterface();
};
