set(TARGET_NAME Test)
file(GLOB_RECURSE PROJ_SOURCES *.cpp *.hpp)

add_executable(${TARGET_NAME} ${PROJ_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SOURCE_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SOURCE_DIR}/Engine)

target_link_libraries(${TARGET_NAME} PUBLIC EngineRuntime)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    target_compile_options(${TARGET_NAME} PRIVATE "/WX") # treat warnings as errors, equivalent to "-Werror"
    target_compile_options(${TARGET_NAME} PRIVATE "/O2")
else() # MinGW
    target_compile_options(${TARGET_NAME} PRIVATE -O2 -Wall -Wextra -Werror)
endif()

install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ./
)
