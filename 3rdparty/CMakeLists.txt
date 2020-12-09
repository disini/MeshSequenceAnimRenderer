cmake_minimum_required(VERSION 2.8)
#project(utils)
set(UTILS_HDRS
        utils/util_init.hpp
        utils/util.hpp
        utils/formatUtils.h
        utils/debugUtils.h
        utils/GlobalManager.h)

set(UTILS_SRCS
        utils/util_init.cpp
        utils/util.cpp
        utils/formatUtils.cpp
        utils/debugUtils.cpp
        utils/GlobalManager.cpp)

#set(STBIMAGE_HDRS
#    stb_image/stb_image.h)

#set(STBIMAGE_SRCS
#        stb_image/stb_image.cpp)

set(TINYOBJ_HDRS
        tinyobj/tiny_obj_loader_me.h)

set(TINYOBJ_SRCS
        tinyobj/tiny_obj_loader_me.cpp)

add_library(utils STATIC
        ${UTILS_HDRS}
        ${UTILS_SRCS}
        )

#add_library(stbimage STATIC
#        ${STBIMAGE_HDRS}
#        ${STBIMAGE_SRCS}
#        )

add_library(tinyobj STATIC
        ${TINYOBJ_HDRS}
        ${TINYOBJ_SRCS}
        )

include_directories(
		/usr/include
		/usr/include/c++/9
		/usr/local/include
		/media/liusheng/Programfiles/Programs/libs/libpng/libpng-1.6.37
)

source_group(src\\ FILES ${UTILS_HDRS} ${UTILS_SRCS})
#source_group(src\\ FILES ${STBIMAGE_HDRS} ${STBIMAGE_SRCS})
source_group(src\\ FILES ${TINYOBJ_HDRS} ${TINYOBJ_SRCS})

set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(tinyobj PROPERTIES LINKER_LANGUAGE CXX)
#set_target_properties(stbimage PROPERTIES LINKER_LANGUAGE CXX)
