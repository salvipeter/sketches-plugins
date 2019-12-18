# -*- mode: Makefile -*-

TEMPLATE = lib
CONFIG += c++14 plugin
QT += widgets
TARGET = surface-plugin

unix:SKETCHES = /home/salvi/project/sketches
# win32:SKETCHES = 

INCLUDEPATH += . /usr/include/eigen3/Eigen $${SKETCHES}/engine/src/Plugins

NBLIB=./InterpolatingBspline

INCLUDEPATH += $${NBLIB}
LIBS += -L$${NBLIB} -lInterpolatingBspline

HEADERS += surface-plugin.hh
SOURCES += surface-plugin.cc
