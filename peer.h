#ifndef PEER_H
#define PEER_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class Client;
class Connection;

class PeerManager : public QObject
{
    Q_OBJECT

public:
    PeerManager(QString &name,Client *client);

    void setPort(int port);
    QByteArray userName() const;
    void startBroadcast();
    bool localHost(const QHostAddress &address);

signals:
    void newConnection(Connection *connection);

private slots:
    void sendDatagram();
    void readDatagram();

private:
    void updateAddresses();

    Client *client;
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QUdpSocket broadcastSocket;
    QTimer broadcastTimer;
    QByteArray username;
    int serverPort;
};

#endif // PEER_H
