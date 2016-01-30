#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QtGui/QDialog>

#include "ui_logindialog.h"
#include "mainwindow.h"

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog, private Ui::LoginDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = 0);
    QString myNick;
    bool getNick;

private slots:
    void on_lineEdit_textEdited(QString );
    void on_pushButton_clicked();
};

#endif // LOGINDIALOG_H
