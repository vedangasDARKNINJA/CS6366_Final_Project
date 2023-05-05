cmake_minimum_required(VERSION 3.15)
                       
## Specify a project name
project( 
    RendererPBR 
    VERSION 1.0
    LANGUAGES CXX
)


## Set the build type
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Release" 
       CACHE STRING "Build configuration 'Release' or 'Debug'."
       FORCE )
endif()


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

## Include Third-Party Libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)

# ASSIMP Options
SET (ASSIMP_BUILD_TESTS OFF)
SET(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
SET(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
SET(ASSIMP_INSTALL OFF)
SET(ASSIMP_BUILD_OBJ_IMPORTER ON)
add_subdirectory(vendor/assimp)

## Include source
add_subdirectory(src)