# -*- mode: Makefile -*-

TEMPLATE = lib
CONFIG += c++14 plugin
QT += widgets
TARGET = surface-plugin
INCLUDEPATH += .

unix:SKETCHES = /home/salvi/project/sketches
win32:SKETCHES = ../../sketches-repository

unix:TRANSFINITE = /home/salvi/project/transfinite
win32:TRANSFINITE = ../../transfinite

INCLUDEPATH += $${SKETCHES}/engine/src/Plugins
INCLUDEPATH += $${TRANSFINITE}/src/geom $${TRANSFINITE}/src/transfinite

unix:LIBS += -L$${TRANSFINITE}/release/geom -L$${TRANSFINITE}/release/transfinite -ltransfinite -lgeom
win32:LIBS += -L$${TRANSFINITE}/src/geom/Release -L$${TRANSFINITE}/src/Release -ltransfinite-free -lgeom

HEADERS += surface-plugin.hh
SOURCES += surface-plugin.cc
