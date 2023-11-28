set(TARGET_NAME Test)
set(EXECUTABLE_OUTPUT_PATH ${PROJ_BINARY_DIR})

file(GLOB_RECURSE TEST_HEADERS *.h)
file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)

add_executable(${TARGET_NAME} ${TEST_HEADERS} ${TEST_SOURCES})

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Test)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SRC_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SRC_DIR}/Engine)

target_link_libraries(${TARGET_NAME} PUBLIC EngineRuntime)

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
