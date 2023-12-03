#pragma once
#include <cstdint>

using Layer = uint32_t;
using LayerMask = uint32_t;

namespace Layers
{
    constexpr Layer Default = 1 << 0;
    // ...
}

namespace LayerMasks
{
    constexpr LayerMask All = ~0;
    constexpr LayerMask None = 0;
    // ...

    bool IsInLayerMask(Layer layer, LayerMask layerMask);
}