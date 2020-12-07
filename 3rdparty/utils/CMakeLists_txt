cmake_minimum_required(VERSION 2.8)
project(utils)

set(UTILS_HDRS
	util_init.hpp
	util.hpp
	formatUtils.h
	debugUtils.h
        GlobalManager.cpp GlobalManager.h)

set(UTILS_SRCS
	util_init.cpp
	util.cpp
	formatUtils.cpp
	debugUtils.cpp
        GlobalManager.cpp GlobalManager.h)

add_library(utils STATIC
	${UTILS_HDRS}
	${UTILS_SRCS}
        GlobalManager.cpp GlobalManager.h)

include_directories(
		/usr/include
		/usr/include/c++/9
		/usr/local/include
		/media/liusheng/Programfiles/Programs/libs/libpng/libpng-1.6.37
)

#include_directories

source_group(src\\ FILES ${UTILS_HDRS} ${UTILS_SRCS})
