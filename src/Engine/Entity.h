#pragma once
#include "Component.h"
#include <bitset>
#include <cstdint>

using Entity = std::uint32_t;

const int MAX_ENTITIES = 5000;

using Signature = std::bitset<MAX_COMPONENTS>;