#pragma once
#include <cstdint>
#include <bitset>
#include "Component.h"

using Entity = std::uint32_t;

const int MAX_ENTITIES = 5000;

using Signature = std::bitset<MAX_COMPONENTS>;