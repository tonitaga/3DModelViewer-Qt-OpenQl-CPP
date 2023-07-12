QT       += core gui openglwidgets

win32: LIBS += -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../controller/viewercontroller.cc \
    ../extern/3rdParty/giflib/dgif_lib.c \
    ../extern/3rdParty/giflib/egif_lib.c \
    ../extern/3rdParty/giflib/gif_err.c \
    ../extern/3rdParty/giflib/gif_font.c \
    ../extern/3rdParty/giflib/gif_hash.c \
    ../extern/3rdParty/giflib/gifalloc.c \
    ../extern/3rdParty/giflib/quantize.c \
    ../extern/gifimage/qgifimage.cpp \
    ../model/facade/viewermodel.cc \
    ../model/helper/FileReader/filechecker.cc \
    ../model/helper/FileReader/filereader.cc \
    ../model/helper/Normalize/normalizer.cc \
    ../model/helper/ObjectRecorder/objectrecorder.cc \
    ../model/helper/TransformModel/transformmodel.cc \
    ../main.cc \
    ../model/helper/VertexData/vertexdata.cc \
    scene.cc \
    view.cc

HEADERS += \
    ../controller/viewercontroller.h \
    ../extern/3rdParty/giflib/gif_hash.h \
    ../extern/3rdParty/giflib/gif_lib.h \
    ../extern/3rdParty/giflib/gif_lib_private.h \
    ../extern/gifimage/qgifglobal.h \
    ../extern/gifimage/qgifimage.h \
    ../extern/gifimage/qgifimage_p.h \
    ../model/facade/viewermodel.h \
    ../model/helper/FileReader/filechecker.h \
    ../model/helper/FileReader/filereader.h \
    ../model/helper/Normalize/normalizer.h \
    ../model/helper/ObjectRecorder/objectrecorder.h \
    ../model/helper/TransformModel/transformmodel.h \
    ../model/helper/VertexData/vertexdata.h \
    scene.h \
    view.h

FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \

RESOURCES += \
    shaders.qrc \
    textures.qrc

SUBDIRS += \

