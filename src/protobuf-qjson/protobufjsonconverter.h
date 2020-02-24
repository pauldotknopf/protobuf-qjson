#ifndef PROTOBUFJSONCONVERTER_H
#define PROTOBUFJSONCONVERTER_H

#include <QJsonValue>
#include <QJSValue>
#include <google/protobuf/message.h>

struct QJSEngine;

class ProtobufJsonConverter
{
public:
    static bool messageToJsonValue(google::protobuf::Message* message, QJsonValue& jsonValue);
    static bool jsonValueToMessage(const QJsonValue& jsonValue, google::protobuf::Message* message);
private:
    ProtobufJsonConverter();
};

#endif // PROTOBUFJSONCONVERTER_H
