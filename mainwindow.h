#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "ui_mainwindow.h"
#include "client.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QString &nick, QWidget *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QString myNickName;
    QTextTableFormat tableFormat;
    Client client;
    bool soundOff;
    bool afk;
    QString quote;
    QString userReady;
    QString userNotReady;
    QString tempMessage;
    QString tempNick;

public slots:
    void addMsg(const QString &from, const QString &message);

private slots:
    void on_actionAbout_activated();
    void on_actionAFK_triggered();
    void on_actionQuot_activated();
    void on_actionSound_triggered();
    void returnPressed();
    void newUser(const QString &nick);
    void userLeft(const QString &nick);
};

#endif // MAINWINDOW_H
