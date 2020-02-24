#include <QtTest>
#include <QCoreApplication>
#include <QJsonValue>
#include "proto/types.pb.h"
#include "protobuf-qjson/protobufjsonconverter.h"

QJsonValue parse(QString str, const char *file, int line)
{
    QJsonValue result;
    if(!ProtobufJsonConverter::stringToJsonValue(str, result)) {
        QTest::qFail("Invalid JSONs tring.", file, line);
    }
    return result;
}

#define PARSE(json) parse(json, __FILE__, __LINE__)

class protobuftests : public QObject
{
    Q_OBJECT

public:
    protobuftests();
    ~protobuftests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void can_deserialize_empty_object();
    void can_not_deserialize_non_object();
    void can_deserialize();
    void round_trip_serialization();
};

protobuftests::protobuftests()
{

}

protobuftests::~protobuftests()
{

}

void protobuftests::initTestCase()
{

}

void protobuftests::cleanupTestCase()
{

}

void protobuftests::can_deserialize_empty_object()
{
    custom::types::TestAllTypes message;
    QJsonValue val = PARSE("{}");
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message));
}

void protobuftests::can_not_deserialize_non_object()
{
    custom::types::TestAllTypes message;
    QJsonValue val;
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message) == false);
    val = QJsonValue::fromVariant(false);
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message) == false);
    val = QJsonValue::fromVariant(34);
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message) == false);
    val = QJsonValue::fromVariant("test");
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message) == false);
}

void protobuftests::can_deserialize()
{
    custom::types::TestAllTypes message;
    QJsonValue val = PARSE("{\"single_int32\": 4}");
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message));
    QCOMPARE(message.single_int32(), 4);

    val = PARSE("{}");
    message.Clear();
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message));
    QCOMPARE(message.has_single_nested_message(), false);

    val = PARSE("{\"single_nested_message\": {}}");
    message.Clear();
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message));
    QCOMPARE(message.has_single_nested_message(), true);
    QCOMPARE(message.single_nested_message().bb(), 0);

    val = PARSE("{\"single_nested_message\": {\"bb\": 45}}");
    message.Clear();
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &message));
    QCOMPARE(message.has_single_nested_message(), true);
    QCOMPARE(message.single_nested_message().bb(), 45);
}

void protobuftests::round_trip_serialization()
{
    custom::types::TestAllTypes message;
    message.set_single_int32(34);
    message.set_single_int64(23);
    message.set_single_uint32(56);
    message.set_single_uint64(456);
    message.set_single_fixed32(2345);
    message.set_single_fixed64(234);
    message.set_single_sfixed32(67);
    message.set_single_sfixed64(98);
    message.set_single_float(354);
    message.set_single_double(3.4);
    message.set_single_bool(true);
    message.set_single_string("sdfdfgsfdgs");
    message.set_single_bytes("234dfg");
    auto nestedMessage = new custom::types::TestAllTypes_NestedMessage();
    nestedMessage->set_bb(5345);
    message.set_allocated_single_nested_message(nestedMessage);
    auto nestedForeignMessage = new custom::types::ForeignMessage();
    nestedForeignMessage->set_c(675);
    message.set_allocated_single_foreign_message(nestedForeignMessage);
    message.set_single_nested_enum(custom::types::TestAllTypes_NestedEnum::TestAllTypes_NestedEnum_FOO);
    message.set_single_foreign_enum(custom::types::ForeignEnum::FOREIGN_FOO);
    message.add_repeated_int32(234234);
    message.set_oneof_string("tests");

    QJsonValue val;
    QVERIFY(ProtobufJsonConverter::messageToJsonValue(&message, val));

    custom::types::TestAllTypes deserialized;
    QVERIFY(ProtobufJsonConverter::jsonValueToMessage(val, &deserialized));

    QCOMPARE(message.single_int32(), deserialized.single_int32());
    QCOMPARE(message.single_int64(), deserialized.single_int64());
    QCOMPARE(message.single_uint32(), deserialized.single_uint32());
    QCOMPARE(message.single_uint64(), deserialized.single_uint64());
    QCOMPARE(message.single_fixed32(), deserialized.single_fixed32());
    QCOMPARE(message.single_fixed64(), deserialized.single_fixed64());
    QCOMPARE(message.single_sfixed32(), deserialized.single_sfixed32());
    QCOMPARE(message.single_sfixed64(), deserialized.single_sfixed64());
    QCOMPARE(message.single_float(), deserialized.single_float());
    QCOMPARE(message.single_double(), deserialized.single_double());
    QCOMPARE(message.single_bool(), deserialized.single_bool());
    QCOMPARE(message.single_string(), deserialized.single_string());
    QCOMPARE(message.single_bytes(), deserialized.single_bytes());
    QCOMPARE(message.has_single_nested_message(), deserialized.has_single_nested_message());
    QCOMPARE(message.single_nested_message().bb(), deserialized.single_nested_message().bb());
    QCOMPARE(message.has_single_foreign_message(), deserialized.has_single_foreign_message());
    QCOMPARE(message.single_foreign_message().c(), deserialized.single_foreign_message().c());
    QCOMPARE(message.single_nested_enum(), deserialized.single_nested_enum());
    QCOMPARE(message.single_foreign_enum(), deserialized.single_foreign_enum());
    QCOMPARE(message.repeated_int32_size(), deserialized.repeated_int32_size());
    QCOMPARE(message.repeated_int32(0), deserialized.repeated_int32(0));
    QCOMPARE(message.oneof_field_case(), deserialized.oneof_field_case());
    QCOMPARE(message.oneof_string(), deserialized.oneof_string());
}

QTEST_MAIN(protobuftests)

#include "tst_protobuftests.moc"
