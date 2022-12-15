# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.10.2)

project("render")

include_directories(${CMAKE_SOURCE_DIR}/third/include)
include_directories(${CMAKE_SOURCE_DIR}/common/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gre/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gre/fw/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/common SRC_COMMON)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine SRC_ENGINE)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gre SRC_GRE)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gre/fw SRC_FW)

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
             ${SRC_GRE}
             ${SRC_FW}
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