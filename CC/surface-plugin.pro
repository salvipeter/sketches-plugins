TEMPLATE = lib
CONFIG += c++14 plugin
QT += widgets
TARGET = surface-plugin
INCLUDEPATH += .

INCLUDEPATH += /home/salvi/project/sketches/engine/src/Plugins

TRANSFINITE = /home/salvi/project/transfinite
INCLUDEPATH += $${TRANSFINITE}/src/geom $${TRANSFINITE}/src/transfinite
LIBS += -L$${TRANSFINITE}/release/geom -L$${TRANSFINITE}/release/transfinite -ltransfinite -lgeom

HEADERS += surface-plugin.hh
SOURCES += surface-plugin.cc
