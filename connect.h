#ifndef CONNECT_H
#define CONNECT_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

static const int MaxBufferSize = 1024000;

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    enum DataType {
        PlainText,
        Greeting,
        Undefined
    };

    Connection(QObject *parent = 0);

    QString name() const;
    void setGreeting(const QString &message);
    bool sendMsg(const QString &message);

signals:
    void readyForUse();
    void newMsg(const QString &from, const QString &message);

protected:
    void timerEvent(QTimerEvent *timerEvent);

private slots:
    void readyRead();
    void sendGreeting();

private:
    int readData(int maxSize = MaxBufferSize);
    int dataLength();
    bool readProtocol();
    bool hasEnoughData();
    void processData();

    QString greetingMsg;
    QString username;
    QByteArray buffer;
    ConnectionState state;
    DataType dataType;
    int bytesForDataType;
    int timerId;
    bool greetingSent;
};

#endif // CONNECT_H
