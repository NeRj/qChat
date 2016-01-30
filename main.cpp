#include <QApplication>
#include <QTranslator>

#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("qchat_ru.qm");
    a.installTranslator(&translator);

    LoginDialog *ld = new LoginDialog();
    ld->exec();
    ld->activateWindow();
    if (ld->getNick){
        MainWindow *mw = new MainWindow(ld->myNick);
        mw->show();
        mw->activateWindow();
    } else exit(0);
    return a.exec();
}
