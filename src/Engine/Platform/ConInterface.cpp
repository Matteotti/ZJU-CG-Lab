#include "ConInterface.h"

#include "Platform/Windows/ConInterfaceWinImpl.h"

ConInterface *ConInterface::getConInterface()
{
#ifdef _WIN32
    static ConInterfaceWinImpl instance;
    return &instance;
#endif
    // implement ConInterfaceLinuxImpl...
}
