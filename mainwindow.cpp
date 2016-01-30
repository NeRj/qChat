#include <QtGui>
#include <QSound>

#include "mainwindow.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QString &nick, QWidget *parent):QMainWindow(parent),client(nick)
{
    userReady = QString(tr("came back..."));
    userNotReady = QString(tr("is AFK..."));
    myNickName = nick;
    setupUi(this);
    soundOff = false;
    afk = false;
    textEdit->setFocus();
    textEdit->installEventFilter(this);
    newUser(myNickName);
    connect(actionSend,SIGNAL(activated()),this,SLOT(returnPressed()));
    connect(&client, SIGNAL(newMsg(QString,QString)),
            this, SLOT(addMsg(QString,QString)));
    connect(&client, SIGNAL(newUser(QString)),
            this, SLOT(newUser(QString)));
    connect(&client, SIGNAL(userLeft(QString)),
            this, SLOT(userLeft(QString)));
    tableFormat.setBorder(0);
}

void MainWindow::addMsg(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    if (message != tempMessage or from != tempNick){
        if (!soundOff && message != userReady && message != userNotReady){
            if (from == myNickName)
                QSound::play("sounds/send.wav");
            else QSound::play("sounds/receive.wav");
        }

        tempMessage = message;
        tempNick = from;

        QTextCursor cursor(plainTextEdit->textCursor());
        cursor.movePosition(QTextCursor::End);
        QTextTable *table = cursor.insertTable(1, 1, tableFormat);
        QDateTime time = QDateTime::currentDateTime();
        if (message == userReady)
            table->cellAt(0, 0).firstCursorPosition().insertHtml(QString(
                    "<font color=\"#00CED1\">* %1 %2</font>").arg(from,message));
        else if (message == userNotReady)
            table->cellAt(0, 0).firstCursorPosition().insertHtml(QString(
                    "<font color=\"#40E0D0\">* %1 %2</font>").arg(from,message));
        else table->cellAt(0, 0).firstCursorPosition().insertHtml(QString(
                "<font color=\"yellow\">%1 [<i>%2</i> ]  </font><font color=\"white\">%3</font>").arg(
                        from, time.toString("hh:mm:ss"),message));
        QScrollBar *bar = plainTextEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
        if (message != userReady && message != userNotReady)
            quote = from + " [" + time.toString("hh:mm:ss") + "] " + message;
    }
}

void MainWindow::returnPressed()
{
    QString text = textEdit->toPlainText();

    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {

        plainTextEdit->appendHtml(tr(
                "<font color=\"#FF6600\">!!! Unknown command: %1</font>").arg(text.left(text.indexOf(' '))));

    } else {
        client.sendMsg(text);
        addMsg(myNickName, text);
    }

    textEdit->clear();
}

void MainWindow::newUser(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = listWidget->findItems(nick,Qt::MatchExactly);
    if (items.isEmpty()){
        if (!soundOff)
            if (nick != myNickName)
                QSound::play("sounds/other.wav");

        listWidget->addItem(nick);
        QList<QListWidgetItem *> items = listWidget->findItems(nick,Qt::MatchExactly);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/icons/src/avatar_32.png"));
        items.at(0)->setIcon(icon);
        plainTextEdit->appendHtml(tr("<font color=\"#99FF00\"><i>* %1 has joined...</i></font>").arg(nick));
    }
}

void MainWindow::userLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    plainTextEdit->appendHtml(tr("<font color=\"red\"><i>* %1 has left...</i></font>").arg(nick));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (event->type() == QEvent::KeyPress && keyEvent->key()
        == Qt::Key_Return && keyEvent->modifiers() != Qt::ShiftModifier){
        returnPressed();
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

void MainWindow::on_actionSound_triggered()
{
    if (soundOff)
        soundOff = false;
    else soundOff = true;
}

void MainWindow::on_actionQuot_activated()
{
    if (quote.isEmpty())
        return;
    textEdit->insertPlainText(">> " + quote);
}

void MainWindow::on_actionAFK_triggered()
{
    if (afk){
        afk = false;
        client.sendMsg(userReady);
        addMsg(myNickName,userReady);
        textEdit->setReadOnly(false);
    } else {
        afk = true;
        client.sendMsg(userNotReady);
        addMsg(myNickName,userNotReady);
        textEdit->setReadOnly(true);
    }
}

void MainWindow::on_actionAbout_activated()
{
    AboutDialog *ad = new AboutDialog();
    ad->exec();
    ad->activateWindow();
}
