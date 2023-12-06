#pragma once

#define ENGINE_WINDOW_TITLE "Engine Window"
#define ENGINE_WINDOW_WIDTH 800
#define ENGINE_WINDOW_HEIGHT 800

#define ENGINE_OPENGL_CONTEXT_VER_MAJOR 4
#define ENGINE_OPENGL_CONTEXT_VER_MINOR 6

#define ENGINE_LOG_PATH "./engine_log.txt"

// Note：优先级数字越小，则越先被 Update。
#define ENGINE_PRIORITY_PHYSICS_SYSTEM 0
#define ENGINE_PRIORITY_TRANSLATE_SYSTEM 1
#define ENGINE_PRIORITY_RENDER_SYSTEM 2
#define ENGINE_PRIORITY_WINDOW_SYSTEM 3
