set(third_party_folder "ThirdParty")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(GLFW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.3.8)
set(GLAD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glad)
set(GLM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glm)
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(${GLFW_PATH})
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

if(NOT TARGET glad)
    include(glad.cmake)
endif()

if(NOT TARGET glm)
    add_subdirectory(glm)
endif()

if(NOT TARGET imgui)
    add_subdirectory(imgui)
endif()
