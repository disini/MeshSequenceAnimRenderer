QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


LIBS += -L/media/liusheng/Programfiles/Programs/libs/libpng/libpng-1.6.37/.libs

LIBS += -ldl -lglfw -lpng -lvulkan


LIBS+= /media/liusheng/Programfiles/Programs/VulkanSDK/1.2.148.1/ubuntu/x86_64/lib/libvulkan.so
LIBS+= /media/liusheng/Programfiles/Programs/libs/glfw/glfw-3.3.2/src/libglfw.so
LIBS+= /usr/lib64/glibc-2.32/lib/libm.so.6
LIBS+= /usr/lib/x86_64-linux-gnu/libxcb.so.1



HEADERS += \
    3rdparty/stb_image/stb_image.h \
    3rdparty/tinyobj/tiny_obj_loader_me.h \
    3rdparty/utils/util.hpp \
    3rdparty/utils/util_init.hpp


SOURCES += \
        3rdparty/utils/util.cpp \
        3rdparty/utils/util_init.cpp \        
        examples/11_Texture_Me/Texture_Me.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += /usr/include \
               /usr/include/c++/9 \
               /usr/local/include \
               /media/liusheng/Programfiles/Programs/libs/glfw/glfw-3.3.2/include \
               /media/liusheng/Programfiles/Programs/libs/glm/0.9.9.8 \
               /media/liusheng/Programfiles/Programs/VulkanSDK/1.2.148.1/ubuntu/x86_64/include \
               /media/liusheng/Programfiles/ubuntu/18.04.5/opt/Qt/5.15.1/gcc_64/include \
               /media/liusheng/Programfiles/Programs/libs/libpng/libpng-1.6.37 \
               3rdparty \
               3rdparty/utils \
               Engine \
               Engine/Monkey \
               external \
               external/assimp/include \
               external/imgui \





CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = MeshSequenceAnimRenderer
    DESTDIR = QT_Creator/x64/Debug
#    DESTDIR = ../x64/Debug
}else{
    TARGET = MeshSequenceAnimRenderer
    DESTDIR = QT_Creator/x64/Release
#    DESTDIR = ../x64/Release
}

DISTFILES += \
    3rdparty/utils/CMakeLists.txt \
    Engine/CMakeLists.txt \
    external/SPIRV-Cross/CMakeLists.txt \
    external/SPIRV-Cross/CODE_OF_CONDUCT.md \
    external/SPIRV-Cross/LICENSE \
    external/SPIRV-Cross/README.md \
    external/SPIRV-Cross/appveyor.yml \
    external/SPIRV-Cross/build_glslang_spirv_tools.sh \
    external/SPIRV-Cross/checkout_glslang_spirv_tools.sh \
    external/SPIRV-Cross/format_all.sh \
    external/SPIRV-Cross/gn/BUILD.gn

