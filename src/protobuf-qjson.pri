QT += quick
CONFIG += c++11 link_pkgconfig

PKGCONFIG += protobuf

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/protobuf-qjson/protobufjsonconverter.h

SOURCES += \
    $$PWD/protobuf-qjson/protobufjsonconverter.cpp
