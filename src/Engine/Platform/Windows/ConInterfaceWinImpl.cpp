#include "ConInterfaceWinImpl.h"

#include <cstdlib>

ConInterfaceWinImpl::ConInterfaceWinImpl()
{
    _handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

ConInterfaceWinImpl::~ConInterfaceWinImpl()
{
    CloseHandle(_handle);
}

void ConInterfaceWinImpl::MoveCursor(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(_handle, coord);
}

void ConInterfaceWinImpl::ClearScreen()
{
    std::system("cls");
}

void ConInterfaceWinImpl::SetTextColor(int color)
{
    SetConsoleTextAttribute(_handle, color);
}
