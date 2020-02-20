#ifndef PROTOBUFJSONCONVERTER_H
#define PROTOBUFJSONCONVERTER_H

#include <QJsonValue>
#include <QJSValue>
#include <google/protobuf/message.h>

struct QJSEngine;

class ProtobufJsonConverter
{
public:
    static bool messageToJsonValue(google::protobuf::Message* message, QJsonValue& jsonValue, QJSEngine* jsEngine);
    static bool messageToJSValue(google::protobuf::Message* message, QJSValue& jsValue, QJSEngine* jsEngine);
    static bool jsonValueToMessage(QJsonValue& jsonValue, google::protobuf::Message* message);
private:
    ProtobufJsonConverter();
};

#endif // PROTOBUFJSONCONVERTER_H
