# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.10.2)

project("render")

include_directories(${CMAKE_SOURCE_DIR}/third/include)
include_directories(${CMAKE_SOURCE_DIR}/common/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/common SRC_COMMON)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine SRC_ENGINE)

add_library(lib-assimp
            SHARED
            IMPORTED)

set_target_properties( # lib name
                       lib-assimp
                       # property
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/third/libs/assimp/${ANDROID_ABI}/libassimp.so)

add_library( # Sets the name of the library.
             lib-render
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             ${SRC_COMMON}
             ${SRC_ENGINE}
             loader.cpp)

target_link_libraries( # Specifies the target library.
                       lib-render
                       # Links the target library to target library.
                       android
                       EGL
                       GLESv2
                       GLESv3
                       jnigraphics
                       lib-assimp
                       log )