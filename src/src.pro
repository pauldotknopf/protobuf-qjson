#QT += quick quick-private

CONFIG += c++11 link_pkgconfig

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

PKGCONFIG += grpc++ protobuf

include(protobuf-qjson.pri)

#SOURCES += \
#        main.cpp \
#        proto-roc/types.pb.cc \
#        proto-roc/gen.pb.cc \
#        proto-roc/gen.grpc.pb.cc

#HEADERS += \
#        proto-roc/types.pb.h \
#        proto-roc/gen.pb.h \
#        proto-roc/gen.grpc.pb.h

#RESOURCES += qml.qrc

SOURCES += \
    main.cpp
