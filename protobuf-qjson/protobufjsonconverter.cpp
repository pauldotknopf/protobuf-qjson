#include <protobuf-qjson/protobufjsonconverter.h>

ProtobufJsonConverter::ProtobufJsonConverter()
{

}

bool ProtobufJsonConverter::messageToJsonValue(google::protobuf::Message* message, QJsonValue& jsonValue)
{
    if(!message) {
        jsonValue = QJsonValue::Null;
        return true;
    }

    return true;
}

bool ProtobufJsonConverter::jsonValueToMessage(QJsonValue& jsonValue, google::protobuf::Message* message)
{
    return true;
}
