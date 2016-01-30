#include "connect.h"

#include <QtNetwork>

static const int TransferTimeout = 30 * 1000;
static const char SeparatorToken = ' ';

Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    greetingMsg = tr("undefined");
    username = tr("unknown");
    state = WaitingForGreeting;
    dataType = Undefined;
    bytesForDataType = -1;
    timerId = 0;
    greetingSent = false;

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(this, SIGNAL(connected()), this, SLOT(sendGreeting()));
}

QString Connection::name() const
{
    return username;
}

void Connection::setGreeting(const QString &message)
{
    greetingMsg = message;
}

bool Connection::sendMsg(const QString &message)
{
    if (message.isEmpty())
        return false;

    QByteArray msg = message.toUtf8();
    QByteArray data = "MESSAGE " + QByteArray::number(msg.size()) + ' ' + msg;
    return write(data) == data.size();
}

void Connection::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == timerId) {
        abort();
        killTimer(timerId);
        timerId = 0;
    }
}

void Connection::readyRead()
{
    if (state == WaitingForGreeting) {
        if (!readProtocol())
            return;
        if (dataType != Greeting) {
            abort();
            return;
        }
        state = ReadingGreeting;
    }

    if (state == ReadingGreeting) {
        if (!hasEnoughData())
            return;

        buffer = read(bytesForDataType);
        if (buffer.size() != bytesForDataType) {
            abort();
            return;
        }

        username = QString::fromUtf8(buffer);
        dataType = Undefined;
        bytesForDataType = 0;
        buffer.clear();

        if (!isValid()) {
            abort();
            return;
        }

        if (!greetingSent)
            sendGreeting();

        state = ReadyForUse;
        emit readyForUse();
    }

    do {
        if (dataType == Undefined) {
            if (!readProtocol())
                return;
        }
        if (!hasEnoughData())
            return;
        processData();
    } while (bytesAvailable() > 0);
}

void Connection::sendGreeting()
{
    QByteArray greeting = greetingMsg.toUtf8();
    QByteArray data = "GREETING " + QByteArray::number(greeting.size()) + ' ' + greeting;
    if (write(data) == data.size())
        greetingSent = true;
}

int Connection::readData(int maxSize)
{
    if (maxSize > MaxBufferSize)
        return 0;

    int numBytesBeforeRead = buffer.size();
    if (numBytesBeforeRead == MaxBufferSize) {
        abort();
        return 0;
    }

    while (bytesAvailable() > 0 && buffer.size() < maxSize) {
        buffer.append(read(1));
        if (buffer.endsWith(SeparatorToken))
            break;
    }
    return buffer.size() - numBytesBeforeRead;
}

int Connection::dataLength()
{
    if (bytesAvailable() <= 0 || readData() <= 0
        || !buffer.endsWith(SeparatorToken))
        return 0;


    buffer.chop(1);
    int number = buffer.toInt();
    buffer.clear();
    return number;
}

bool Connection::readProtocol()
{
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }

    if (readData() <= 0) {
        timerId = startTimer(TransferTimeout);
        return false;
    }

    if (buffer == "MESSAGE ") {
        dataType = PlainText;
    } else if (buffer == "GREETING ") {
        dataType = Greeting;
    } else {
        dataType = Undefined;
        abort();
        return false;
    }

    buffer.clear();
    bytesForDataType = dataLength();
    return true;
}

bool Connection::hasEnoughData()
{
    if (timerId) {
        QObject::killTimer(timerId);
        timerId = 0;
    }

    if (bytesForDataType <= 0)
        bytesForDataType = dataLength();

    if (bytesAvailable() < bytesForDataType
        || bytesForDataType <= 0) {
        timerId = startTimer(TransferTimeout);
        return false;
    }

    return true;
}

void Connection::processData()
{
    buffer = read(bytesForDataType);
    if (buffer.size() != bytesForDataType) {
        abort();
        return;
    }

    switch (dataType) {
    case PlainText:
        emit newMsg(username, QString::fromUtf8(buffer));
        break;
    default:
        break;
    }

    dataType = Undefined;
    bytesForDataType = 0;
    buffer.clear();
}
