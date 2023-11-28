set(TARGET_NAME EngineRuntime)

file(GLOB_RECURSE ENGINE_HEADERS *.h)
file(GLOB_RECURSE ENGINE_SOURCES *.cpp *.hpp)

add_library(${TARGET_NAME} ${ENGINE_HEADERS} ${ENGINE_SOURCES})

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Engine)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SRC_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glm)

if(MSVC)

    target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    target_compile_options(${TARGET_NAME} PRIVATE "/WX-")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${TARGET_NAME} PRIVATE "/O2")
    endif()

else() # MinGW(gcc/g++)

    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${TARGET_NAME} PRIVATE "-O2")
    endif()

endif()

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJ_ROOT_DIR}/assets 
                ${PROJ_ASSETS_DIR})
