#ifndef PROTOBUFJSONCONVERTER_H
#define PROTOBUFJSONCONVERTER_H

#include <QJsonValue>
#include <google/protobuf/message.h>

class ProtobufJsonConverter
{
public:
    static bool messageToJsonValue(google::protobuf::Message* message, QJsonValue& jsonValue);
    static bool jsonValueToMessage(QJsonValue& jsonValue, google::protobuf::Message* message);
private:
    ProtobufJsonConverter();
};

#endif // PROTOBUFJSONCONVERTER_H
