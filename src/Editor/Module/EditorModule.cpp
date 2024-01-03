#include "EditorModule.h"
#include "Entity.h"

#include <cstdint>

std::shared_ptr<std::vector<Entity>> EditorModule::_entities;
Entity EditorModule::_selectedEntity = UINT32_MAX;
