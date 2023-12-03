#include "Layer.h"

bool LayerMasks::IsInLayerMask(Layer layer, LayerMask layerMask)
{
    return (layerMask & layer) != 0;
}