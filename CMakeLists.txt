cmake_minimum_required(VERSION 3.20)
project(TempoUI)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h" "src/*.rc")

list(APPEND PROJECT_SOURCES "vendor/glad.c")

set_source_files_properties("vendor/glad.c" PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

option(TEMPO_USE_GLFW "Use GLFW for inputs" ON)
option(TEMPO_USE_OPENGL "Use OpenGL as rendering backend" ON)
option(TEMPO_USE_LAYOUTS "Enable JSON layout loading" ON)

if(TEMPO_USE_GLFW)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)
endif()

set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG VER-2-14-1
)

FetchContent_MakeAvailable(freetype)

if(CMAKE_BUILD_TYPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif()

add_library(${PROJECT_NAME} OBJECT ${PROJECT_SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

target_include_directories(${PROJECT_NAME} 
        PUBLIC
                ${PROJECT_SOURCE_DIR}/vendor
                ${PROJECT_SOURCE_DIR}/src
                ${PROJECT_SOURCE_DIR}/src/TempoUI
        PRIVATE
                ${PROJECT_SOURCE_DIR}/src/TempoUI/PCH
)

if(EXISTS "${PROJECT_SOURCE_DIR}/src/TempoUI/PCH/PCH.h")
    target_precompile_headers(${PROJECT_NAME} PRIVATE src/TempoUI/PCH/PCH.h)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
        freetype
)

if(TEMPO_USE_GLFW)
target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_UI)
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
)
endif()
if(TEMPO_USE_OPENGL)
target_compile_definitions(${PROJECT_NAME} PUBLIC OPENGL)
endif()
if(TEMPO_USE_LAYOUTS)
target_compile_definitions(${PROJECT_NAME} PUBLIC LAYOUT_LOADER)
endif()


function(TempoUI_Resources TARGET_NAME)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TempoUI_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/resources"
        )
endfunction()