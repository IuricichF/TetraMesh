TEMPLATE = app

CONFIG -= app_bundle
CONFIG -= qt

TARGET = TetraMesh

# Directories
DESTDIR = dist/
OBJECTS_DIR = build/

QMAKE_CXXFLAGS_RELEASE += -fpermissive
QMAKE_CXXFLAGS_DEBUG += -fpermissive
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3


mac : {
QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_LFLAGS += -lstdc++
}


HEADERS += \
    source/Define.h \
    source/Edge.h \
    source/Mesh.h \
    source/Tetra.h \
    source/Triangle.h \
    source/Vertex.h

SOURCES += \
    source/Mesh.cpp \
    source/IO.cpp \
    main.cpp

