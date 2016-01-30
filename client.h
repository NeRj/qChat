#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>

#include "server.h"

class PeerManager;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QString &name, QObject *parent = 0);
    QString nickName;
    void sendMsg(const QString &message);
    bool hasConnect(const QHostAddress &senderIp, int senderPort = -1) const;

signals:
    void newMsg(const QString &from, const QString &message);
    void newUser(const QString &nick);
    void userLeft(const QString &nick);

private slots:
    void newConnection(Connection *connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();

private:
    void removeConnection(Connection *connection);

    PeerManager *peerManager;
    Server server;
    QMultiHash<QHostAddress, Connection *> peers;
};

#endif // CLIENT_H
