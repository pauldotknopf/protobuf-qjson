#include <protobuf-qjson/protobufjsonconverter.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/type_resolver_util.h>
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

bool ProtobufJsonConverter::messageToJsonValue(google::protobuf::Message* message, QJsonValue& jsonValue, QJSEngine* jsEngine)
{
    std::string output;
    google::protobuf::util::JsonOptions jsonOptions;
    auto result = google::protobuf::util::MessageToJsonString(*message, &output, jsonOptions);
    if(!result.ok()) {
        qCritical("Message to json error: %s", result.error_message().data());
        jsonValue = QJsonValue::Undefined;
        return false;
    }

    QJSValue func = jsEngine->evaluate("JSON.parse");
    auto jsValue = func.call(QJSValueList{ QString::fromStdString(output) });

    // TODO: Check for errors
    if(jsValue.isError()) {
        qCritical("JSON.parse serror");
        jsonValue = QJsonValue::Undefined;
        return false;
    }

    qDebug() << jsValue.isObject();

    return true;
}

bool ProtobufJsonConverter::messageToJSValue(google::protobuf::Message* message, QJSValue& jsValue, QJSEngine* jsEngine)
{
    std::string output;
    google::protobuf::util::JsonOptions jsonOptions;
    auto result = google::protobuf::util::MessageToJsonString(*message, &output, jsonOptions);
    if(!result.ok()) {
        qCritical("Message to json error: %s", result.error_message().data());
        jsValue = QJSValue::UndefinedValue;
        return false;
    }

    QJSValue func = jsEngine->evaluate("JSON.parse");
    jsValue = func.call(QJSValueList{ QString::fromStdString(output) });

    // TODO: Check for errors
    if(jsValue.isError()) {
        qCritical("JSON.parse serror");
        jsValue = QJSValue::UndefinedValue;
        return false;
    }

    return true;
}

bool ProtobufJsonConverter::jsonValueToMessage(QJsonValue& jsonValue, google::protobuf::Message* message)
{
    auto object = jsonValue.toObject();
    QJsonDocument doc(object);
    QByteArray bytes = doc.toJson();

    auto url = std::string("type.googleapis.com/");
    url.append(message->GetDescriptor()->full_name());
    auto typeResolver = google::protobuf::util::NewTypeResolverForDescriptorPool("type.googleapis.com", message->GetDescriptor()->file()->pool());

    google::protobuf::io::ArrayInputStream inputStream(bytes.data(), bytes.size());
    QByteArray byteArray;
    QByteArrayOutputStream outputStream(&byteArray);

    google::protobuf::util::JsonParseOptions parseOptions;
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
