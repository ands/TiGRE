TEMPLATE = lib
CONFIG += staticlib
QT -= core gui
QT += opengl

unix : !mac {
    LIBS += -lglut -lGLU -lopenal -lalut
    QMAKE_CXXFLAGS += -std=c++11
}

mac {
    CONFIG -= app_bundle
    LIBS += -framework GLUT OpenAL alut
}

win32 {
    # GLUT
    GLUTDIR = libs\freeglut
    INCLUDEPATH += $$GLUTDIR/include
    LIBS += $$GLUTDIR/lib/freeglut.lib
    
    # GLEW
    GLEWDIR = libs\glew
    INCLUDEPATH += $$GLEWDIR/include
    LIBS += $$GLEWDIR/lib/glew32.lib
    
    # OpenAL
    ALDIR = libs\al
    INCLUDEPATH += $$ALDIR/include
    LIBS += $$ALDIR/libs/Win32/OpenAL32.lib
    
    # ALUT
    ALUTDIR = libs\alut
    INCLUDEPATH += $$ALUTDIR/include
    LIBS += $$ALUTDIR/lib/alut.lib
}

INCLUDEPATH += libs/glm/include
INCLUDEPATH += libs/lodepng
INCLUDEPATH += libs/pugixml
INCLUDEPATH += include

SOURCES += libs/pugixml/pugixml.cpp
SOURCES += libs/lodepng/lodepng.cpp
SOURCES += src/GUISurface.cpp
SOURCES += src/Scene.cpp
SOURCES += src/GUITileSurface.cpp
SOURCES += src/GUIControl.cpp
SOURCES += src/Shader.cpp
SOURCES += src/Object.cpp
SOURCES += src/GUIWindow.cpp
SOURCES += src/Buffer.cpp
SOURCES += src/Sound.cpp
SOURCES += src/GUILabel.cpp
SOURCES += src/Material.cpp
SOURCES += src/Texture.cpp
SOURCES += src/Light.cpp
SOURCES += src/Mesh.cpp
SOURCES += src/Billboard.cpp
SOURCES += src/GUIButton.cpp
SOURCES += src/Resource.cpp
SOURCES += src/Camera.cpp
SOURCES += src/Resources.cpp
SOURCES += src/GUI.cpp
SOURCES += src/Renderer.cpp
