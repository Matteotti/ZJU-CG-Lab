set(third_party_folder "ThirdParty")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw-3.3.8)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

if(NOT TARGET glad)
    include(glad.cmake)
endif()

if(NOT TARGET glm)
    add_subdirectory(glm)
endif()
