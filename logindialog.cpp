#include <QtGui>

#include "logindialog.h"
#include "ui_logindialog.h"
//#include "mainwindow.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    pushButton->setDisabled(true);
    getNick = false;
}

void LoginDialog::on_pushButton_clicked()
{
    myNick = lineEdit->text();
    getNick = true;
    this->close();
}

void LoginDialog::on_lineEdit_textEdited(QString )
{
    if(!lineEdit->text().isEmpty())
        pushButton->setEnabled(true);
    else pushButton->setDisabled(true);
}
