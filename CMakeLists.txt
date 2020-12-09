cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(MeshSequenceAnimRenderer)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -DNOMINMAX=1 -fPIC")
#set（CMAKE_C_FLAGS“$ {CMAKE_C_FLAGS} -fPIC”）
#set（CMAKE_CXX_FLAGS“$ {CMAKE_CXX_FLAGS} -fPIC”）

# 修改clion 内存
#MATH(EXPR stack_size "64*1024*1024")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,-stack,${stack_size}")

# res data
add_definitions(-DDEMO_RES_PATH=\"${CMAKE_SOURCE_DIR}/examples/\")

add_definitions(-fPIC)

add_compile_options(-fPIC)

if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_WINDOWS=1 -DMONKEY_DEBUG=1")
#        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libvulkan -static-libQt5Core")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(Vulkan_LIBRARY NAMES vulkan-1 HINTS "${CMAKE_SOURCE_DIR}/external/vulkan/windows/libs/x64" REQUIRED)
	else (CMAKE_SIZEOF_VOID_P EQUAL 8)
		find_library(Vulkan_LIBRARY NAMES vulkan-1 HINTS "${CMAKE_SOURCE_DIR}/external/vulkan/windows/libs/x86" REQUIRED)
	endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
	include_directories(
		external/vulkan/windows/include/
	)
elseif (IOS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_IOS=1")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal -framework Foundation -framework QuartzCore -framework IOKit -framework UIKit -framework IOSurface")
	set(Vulkan_LIBRARY "${CMAKE_SOURCE_DIR}/external/vulkan/ios/lib/libMoltenVK.a")
	include_directories(
		external/vulkan/ios/include/
	)
elseif (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_MAC=1 -DMONKEY_DEBUG=1")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework QuartzCore")
	set(CMAKE_XCODE_GENERATE_SCHEME ON)
	set(XCODE_SCHEME_ENVIRONMENT ON)
	find_library(Vulkan_LIBRARY NAMES vulkan HINTS "${CMAKE_SOURCE_DIR}/external/vulkan/macos/lib" REQUIRED)
	include_directories(
		external/vulkan/macos/include/
	)
elseif (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_LINUX=1 -DMONKEY_DEBUG=1")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
#        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libvulkan -static-libQt5Core")
	find_library(Vulkan_LIBRARY NAMES vulkan HINTS "${CMAKE_SOURCE_DIR}/external/vulkan/linux/lib" REQUIRED)
	include_directories(
		external/vulkan/linux/include/
	)
	find_package(XCB REQUIRED)
	include_directories(
		${XCB_INCLUDE_DIRS}
	)
endif ()

set(ALL_LIBS
	${Vulkan_LIBRARY}
	vulkan
	imgui
	assimp
	spirv-cross-glsl
	spirv-cross-hlsl
	spirv-cross-cpp
	spirv-cross-reflect
	spirv-cross-msl
	spirv-cross-util
	spirv-cross-core
	Monkey
        Qt5Core
        icui18n
#        utils
#        tinyobj
#        stbimage
	png
)

if (UNIX AND NOT APPLE)
	set(ALL_LIBS
		${ALL_LIBS}
		${XCB_LIBRARIES}
	)
endif ()

include_directories(
	Engine/Monkey/
	3rdparty/
	3rdparty/utils/
#        3rdparty/stb_image/
        3rdparty/tinyobj/
	external/imgui/
	external/SPIRV-Cross/
	external/assimp/include/
        /media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/include/
#    /usr/include/x86_64-linux-gnu/qt5
)

link_directories(
    /media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib/
)
add_definitions("-Wl,-rpath=/media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib/")

add_subdirectory(external/imgui)
add_subdirectory(external/SPIRV-Cross)
add_subdirectory(external/assimp)
add_subdirectory(Engine)
add_subdirectory(3rdparty)
#add_subdirectory(3rdparty/utils)
#add_subdirectory(3rdparty/stb_image)
#add_subdirectory(3rdparty/tinyobj)
add_subdirectory(examples)

#target_link_libraries(/media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib/libQt5Core.so)

#target_link_libraries(/media/liusheng/Programfiles/Programs/libs/libpng/1.6.37/lib/libpng.a)
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/myUser/Demo/bin



set(INSTALL_LIB_DIR "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_INSTALL_RPATH "/media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib")
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath,"/media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib/libQt5Core.so")

#set(CMAKE_BUILD_RPATH )
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES ${CMAKE_INSTALL_RPATH} isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${INSTALL_LIB_DIR}")
endif("${isSystemDir}" STREQUAL "-1")




