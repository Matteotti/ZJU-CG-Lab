set(TARGET_NAME EngineRuntime)
file(GLOB_RECURSE ENGINE_SOURCES *.cpp *.hpp)

add_library(${TARGET_NAME} ${ENGINE_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_SOURCE_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glm)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    target_compile_options(${TARGET_NAME} PRIVATE "/WX") # treat warnings as errors, equivalent to "-Werror"
    target_compile_options(${TARGET_NAME} PRIVATE "/O2")
else() # MinGW
    target_compile_options(${TARGET_NAME} PRIVATE -O2 -Wall -Wextra -Werror)
endif()

install(TARGETS ${TARGET_NAME} glfw glad
    RUNTIME DESTINATION ./
)
