# CMake Integration
TempoUI is designed to be easily integrated into modern C++ projects using CMake's FetchContent functionality. By utilizing FetchContent every dependency of TempoUI (GLFW, FreeType) will be automatically handled, and your current application will be set to the correct standard for utilizing the library.

### System Requirements
* **CMake:** Version 3.20 or higher.
* **Compiler: ** Full **C++ 23** support is required

### Step 1: CMakeLists.txt template

Adding TempoUI to a CMake project is simple and only requires a few lines of code.

Here is a minimal CMakeLists.txt template you can use as a start.

```cmake
cmake_minimum_required(VERSION 3.20)
project(AppName)

# Even though enforced by the library, setting standard keeps it clear
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetching
include(FetchContent)
FetchContent_Declare(
	TempoUI
	GIT_REPOSITORY [https://github.com/DKZHD/TempoUI.git](https://github.com/DKZHD/TempoUI.git)
	GIT_TAG main # or version number
)
FetchContent_MakeAvailable(TempoUI)

add_executable(${PROJECT_NAME} main.cpp)

# You can also add `glfw` or `freetype` here as they are both fetched by TempoUI
target_link_libraries(${PROJECT_NAME} PRIVATE TempoUI)

# Optional additional resources (two fonts and default theme) can be acquired by using TempoUI_Resources 
TempoUI_Resources(${PROJECT_NAME})
```

### Step 2: Include Paths
TempoUI utilizes a clean, namespaced master header to prevent global scope pollution.
``` cpp
// Correct Usage:
#include <TempoUI/TempoUI.h>

// Do not use this (Even though it will not break your project)
#include "TempoUI.h"
#include "Elements/Button/Button.h"
// etc...
```

### Step 3: Opting out of Options (*Optional*)
TempoUI has 3 options that can be toggled to change the behavior and compilation of the library.
All of these options are `ON` by default, meaning you have to opt out yourself.
>[!WARNING]
>To override these options, these variables **must** be set before fetching the library!

``` cmake
set(TEMPO_USE_GLFW OFF) # Disables GLFW for input and will not fetch it either so be careful if relying on TempoUI to fetch for you

set(TEMPO_USE_OPENGL OFF) # Disables OpenGL as the rendering backend

set(TEMPO_USE_LAYOUTS OFF) # Disables JSON layout loading
```

