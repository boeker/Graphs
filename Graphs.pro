TEMPLATE = app
TARGET = 
DEPENDPATH += . graph graph/exceptions
INCLUDEPATH += .
CONFIG += qt exceptions

# Input
HEADERS += mainwindow.h \
           graph/edge.h \
           graph/graph.h \
           graph/node.h \
           graph/exceptions/graphfileerror.h
SOURCES += main.cpp \
           mainwindow.cpp \
           graph/edge.cpp \
           graph/graph.cpp \
           graph/node.cpp \
           graph/exceptions/graphfileerror.cpp

TRANSLATIONS = graphs_de.ts
