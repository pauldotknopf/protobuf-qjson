QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

include(../src/protobuf-qjson.pri)

SOURCES +=  tst_protobuftests.cpp \
    proto/types.pb.cc

HEADERS += \
    proto/types.pb.h
