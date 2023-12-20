#pragma once
#include "Component.h"
#include <bitset>
#include <cstdint>

using Entity = std::uint32_t;

constexpr int MAX_ENTITIES = 100000;

using Signature = std::bitset<MAX_COMPONENTS>;