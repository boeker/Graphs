TEMPLATE = app
TARGET = 
DEPENDPATH += . graph graph/exceptions
INCLUDEPATH += .
CONFIG += qt exceptions

# Input
HEADERS += gui/mainwindow.h \
           gui/resultdialog.h \
           graph/edge.h \
           graph/graph.h \
           graph/node.h \
           graph/exceptions/graphfileerror.h
SOURCES += main.cpp \
           gui/mainwindow.cpp \
           gui/resultdialog.cpp \
           graph/edge.cpp \
           graph/graph.cpp \
           graph/node.cpp \
           graph/exceptions/graphfileerror.cpp

TRANSLATIONS = translations/graphs_de.ts
