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

add_library(utils STATIC
	${UTILS_HDRS}
	${UTILS_SRCS}
        )

include_directories(
		/usr/include
		/usr/include/c++/9
		/usr/local/include
		/media/liusheng/Programfiles/Programs/libs/libpng/libpng-1.6.37
)

#include_directories

source_group(src\\ FILES ${UTILS_HDRS} ${UTILS_SRCS})
