MACRO(SETUP_SAMPLE_START EXE_NAME)
	UNSET(SOURCE_FILES)
	UNSET(HEADER_FILES)
	UNSET(RESOURCE_FILES)
	UNSET(DEFINITIONS)
ENDMACRO(SETUP_SAMPLE_START)

MACRO(SETUP_SAMPLE_END EXE_NAME)
	SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
	SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
	SOURCE_GROUP(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Resources" FILES ${RESOURCE_FILES})

	ADD_EXECUTABLE(${EXE_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${EFFECT_FILES})
	SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES FOLDER examples)

if (IOS)
	# set_source_files_properties( 
	# 	star.obj        # 3D model as OBJ txt file
	# 	star.off        # 3D model as OFF txt file
	# 	PROPERTIES 
	# 	HEADER_FILE_ONLY ON 
	# ) 
	SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES
		MACOSX_BUNDLE TRUE
		RESOURCE "${RESOURCE_FILES}"
	)
elseif (APPLE)
	SET(MACOS_ENVIRONMENT "VK_LAYER_PATH=${CMAKE_SOURCE_DIR}/external/vulkan/macos/etc/vulkan/explicit_layer.d;VK_ICD_FILENAMES=${CMAKE_SOURCE_DIR}/external/vulkan/macos/etc/vulkan/icd.d/MoltenVK_icd.json")
	SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES XCODE_SCHEME_ENVIRONMENT "${MACOS_ENVIRONMENT}")
elseif(WIN32)
	SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug")
	SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES VS_RELEASE_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug")
endif()
	
	TARGET_COMPILE_DEFINITIONS(${EXE_NAME} PRIVATE ${DEFINITIONS})
	SET(EXTRA_LINKED_LIBRARIES 
		${EXTRA_LINKED_LIBRARIES}
		${ALL_LIBS}
#                ${CMAKE_SOURCE_DIR}/qt-build-debug/external/assimp/code
#                ${CMAKE_SOURCE_DIR}/qt-build-debug/external/imgui
#                /media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib/libQt5Core.so
#                /media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/lib/libicui18n.so.56
	)

        message(status "${ALL_LIBS}")
        message(status "${EXTRA_LINKED_LIBRARIES}")

	TARGET_LINK_LIBRARIES(${EXE_NAME} ${EXTRA_LINKED_LIBRARIES})
ENDMACRO(SETUP_SAMPLE_END)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	SET(MainLaunch
		${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Monkey/Launch/LaunchWindows.cpp
	)
elseif (IOS)
	SET(MainLaunch
		${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Monkey/Launch/LaunchIOS.cpp
	)
	set_source_files_properties(
		${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Monkey/Launch/LaunchIOS.cpp
		PROPERTIES 
        COMPILE_FLAGS "-x objective-c++"
	)
elseif (APPLE)
	SET(MainLaunch
		${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Monkey/Launch/LaunchMac.cpp
	)
	set_source_files_properties(
		${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Monkey/Launch/LaunchMac.cpp
		PROPERTIES 
		COMPILE_FLAGS "-x objective-c++"
	)
elseif (UNIX)
	SET(MainLaunch
		${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Monkey/Launch/LaunchLinux.cpp
	)
endif ()



SETUP_SAMPLE_START(MSARLS)
        SET(HEADER_FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/formatUtils.h
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/util.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/util_init.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/GlobalManager.h
                )
	SET(SOURCE_FILES
		${MainLaunch}
		${CMAKE_CURRENT_SOURCE_DIR}/MSARLS/MSARLS.cpp                
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/formatUtils.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/util.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/util_init.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/utils/GlobalManager.cpp
	)
	file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/MSARLS/*.*")
	foreach(file ${files})
		SET(ASSETS
			${ASSETS}
			${file}
		)
	endforeach()
	SET(RESOURCE_FILES ${ASSETS})
SETUP_SAMPLE_END(MSARLS)

