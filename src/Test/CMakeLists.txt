set(TARGET_NAME Test)
set(EXECUTABLE_OUTPUT_PATH ${PROJ_BINARY_DIR})

file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)

add_executable(${TARGET_NAME} ${TEST_SOURCES})

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Test)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SRC_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SRC_DIR}/Engine)

target_link_libraries(${TARGET_NAME} PUBLIC EngineRuntime)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    target_compile_options(${TARGET_NAME} PRIVATE "/WX-")
    target_compile_options(${TARGET_NAME} PRIVATE "/O2")
else() # MinGW
    target_compile_options(${TARGET_NAME} PRIVATE -O2 -Wall -Wextra)
endif()

#debug info
message(STATUS "Copying assets to ${PROJ_BINARY_DIR}")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJ_ROOT_DIR}/assets 
                ${PROJ_BINARY_DIR})
