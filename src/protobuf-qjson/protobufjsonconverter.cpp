#include <protobuf-qjson/protobufjsonconverter.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <QDebug>
#include <QJSEngine>
#include <QJSValueList>
#include <QJsonDocument>
#include <QJsonObject>

struct QByteArrayOutputStream : google::protobuf::io::ZeroCopyOutputStream {
    QByteArrayOutputStream(QByteArray* byteArray) : _byteArray(byteArray), _position(0) {
    }
    bool Next(void** data, int* size) override;
    void BackUp(int count) override;
    int64_t ByteCount() const override;
    QByteArray* _byteArray;
    int _position;
};

bool QByteArrayOutputStream::Next(void** data, int* size) {
    const int bufferSize = 1024;
    *size = bufferSize;
    _position += bufferSize;
    if(_byteArray->length() < _position) {
        _byteArray->resize(_position);
    }
    *data = static_cast<void*>(&_byteArray->data()[_position-bufferSize]);
    return true;
}
void QByteArrayOutputStream::BackUp(int count) {
    _position -= count;
}
int64_t QByteArrayOutputStream::ByteCount() const {
    return _position;
}

ProtobufJsonConverter::ProtobufJsonConverter()
{

}

bool ProtobufJsonConverter::messageToJsonValue(google::protobuf::Message* message, QJsonValue& jsonValue)
{
    std::string json;
    if(!messageToJsonString(message, json)) {
        qCritical("couldn't convert message to json.");
        return false;
    }

    return stringToJsonValue(json, jsonValue);
}

bool ProtobufJsonConverter::messageToJsonString(google::protobuf::Message* message, std::string& jsonString)
{
    auto typeResolver = google::protobuf::util::NewTypeResolverForDescriptorPool("type.googleapis.com", google::protobuf::DescriptorPool::generated_pool());
    std::string binaryString;
    if(!message->SerializeToString(&binaryString)) {
        qDebug("couldn't serialize message to string");
        return false;
    }

    auto url = std::string("type.googleapis.com/");
    url.append(message->GetDescriptor()->full_name());
    google::protobuf::util::JsonPrintOptions options;
    options.always_print_primitive_fields = true;
    auto status = google::protobuf::util::BinaryToJsonString(typeResolver, url, binaryString, &jsonString, options);
    if(!status.ok()) {
        qCritical("error converting message to json");
        return false;
    }

    return true;
}

bool ProtobufJsonConverter::jsonValueToMessage(const QJsonValue& jsonValue, google::protobuf::Message* message, bool ignoreInvalidFields)
{
    if(jsonValue.type() != QJsonValue::Object) {
        qCritical("invalid json value type: %d", jsonValue.type());
        return false;
    }

    auto object = jsonValue.toObject();
    QJsonDocument doc(object);
    QByteArray bytes = doc.toJson();

    auto typeResolver = google::protobuf::util::NewTypeResolverForDescriptorPool("type.googleapis.com", message->GetDescriptor()->file()->pool());

    google::protobuf::io::ArrayInputStream inputStream(bytes.data(), bytes.size());
    QByteArray byteArray;
    QByteArrayOutputStream outputStream(&byteArray);

    google::protobuf::util::JsonParseOptions parseOptions;
    parseOptions.ignore_unknown_fields = ignoreInvalidFields;
    auto url = std::string("type.googleapis.com/");
    url.append(message->GetDescriptor()->full_name());
    auto result = google::protobuf::util::JsonToBinaryStream(typeResolver, url, &inputStream, &outputStream, parseOptions);
    if(!result.ok()) {
        qCritical("Json to message error: %s", result.error_message().data());
        return false;
    }

    if(!message->ParseFromArray(byteArray.data(), static_cast<int>(outputStream.ByteCount()))) {
        qCritical("Couldn't parse message from byte array.");
        return false;
    }

    return true;
}

bool ProtobufJsonConverter::stringToJsonValue(const QString& str, QJsonValue& jsonValue)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            auto obj = doc.object();
            jsonValue = QJsonValue::fromVariant(obj);
            return true;
        }
    }

    qCritical("failed to convert to json object: %s", qPrintable(str));
    jsonValue = QJsonValue::Undefined;
    return false;
}

bool ProtobufJsonConverter::stringToJsonValue(const std::string& str, QJsonValue& jsonValue)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.c_str());
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            auto obj = doc.object();
            jsonValue = QJsonValue::fromVariant(obj);
            return true;
        }
    }

    qCritical("failed to convert to json object: %s", str.c_str());
    jsonValue = QJsonValue::Undefined;
    return false;
}
