set(TARGET_NAME Test)
file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)

add_executable(${TARGET_NAME} ${TEST_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SOURCE_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SOURCE_DIR}/Engine)

target_link_libraries(${TARGET_NAME} PUBLIC EngineRuntime)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    target_compile_options(${TARGET_NAME} PRIVATE "/WX-")
    target_compile_options(${TARGET_NAME} PRIVATE "/O2")
else() # MinGW
    target_compile_options(${TARGET_NAME} PRIVATE -O2 -Wall -Wextra)
endif()

install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ./
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJ_ROOT_DIR}/assets 
                ${CMAKE_INSTALL_PREFIX})
