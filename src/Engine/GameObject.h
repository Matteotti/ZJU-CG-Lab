#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"

int g_minAvailuableID = 0;

class GameObject;

std::map<int, GameObject *> g_gameObjects;

std::multimap<int, int> g_disableZBufferList;

std::multimap<int, int> g_enableZBufferList;