set(TARGET_NAME imgui)

file(GLOB_RECURSE IMGUI_SOURCES *.cpp)

add_library(${TARGET_NAME} ${IMGUI_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${GLFW_PATH}/include)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
