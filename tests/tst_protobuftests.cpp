#include <QtTest>
#include <QCoreApplication>

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

}

QTEST_MAIN(protobuftests)

#include "tst_protobuftests.moc"
