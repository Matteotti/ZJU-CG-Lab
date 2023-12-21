set(TARGET_NAME imgui)

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.h ${IMGUI_PATH}/*.cpp)

add_library(${TARGET_NAME} ${IMGUI_SOURCES}
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_glfw.h
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${IMGUI_PATH})
target_include_directories(${TARGET_NAME} PUBLIC ${IMGUI_PATH}/backends)
target_include_directories(${TARGET_NAME} PUBLIC ${GLFW_PATH}/include)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
