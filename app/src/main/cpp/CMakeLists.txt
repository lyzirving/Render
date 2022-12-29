# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.10.2)

project("render")

include_directories(${CMAKE_SOURCE_DIR}/third/include)
include_directories(${CMAKE_SOURCE_DIR}/common/include)
include_directories(${CMAKE_SOURCE_DIR}/common/tools/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gre/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gre/fw/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gfx/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gfx/egl/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/gfx/mgr/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/view/base/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/view/conv/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/view/layers/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/view/layers/items/include)
include_directories(${CMAKE_SOURCE_DIR}/engine/view/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/common SRC_COMMON)
aux_source_directory(${CMAKE_SOURCE_DIR}/common/tools SRC_TOOLS)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine SRC_ENGINE)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gre SRC_GRE)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gre/fw SRC_FW)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gfx SRC_GFX)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gfx/egl SRC_EGL)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/gfx/mgr SRC_GFX_MGR)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/view/base SRC_BASE_VIEW)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/view/conv SRC_CONV_VIEW)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/view/layers SRC_LAYERS_VIEW)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/view/layers/items SRC_ITEMS_VIEW)
aux_source_directory(${CMAKE_SOURCE_DIR}/engine/view SRC_VIEW)

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
             ${SRC_TOOLS}
             ${SRC_ENGINE}
             ${SRC_GRE}
             ${SRC_FW}
             ${SRC_GFX}
             ${SRC_EGL}
             ${SRC_GFX_MGR}
             ${SRC_VIEW}
             ${SRC_BASE_VIEW}
             ${SRC_CONV_VIEW}
             ${SRC_ITEMS_VIEW}
             ${SRC_LAYERS_VIEW}
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