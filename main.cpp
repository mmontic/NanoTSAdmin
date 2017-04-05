#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSettings>

#include "main.h"
#include "meditsettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("MMSoft");
    QApplication::setApplicationName("nanoTSAdmin");

    QSettings settings;

    if (!settings.contains("ID"))
    {
        MEditSettings editor;
        if (!editor.exec())
            return 2;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("HostName","").toString());
    db.setDatabaseName(settings.value("DatabaseName","").toString());
    db.setUserName(settings.value("UserName","").toString());
    db.setPassword(settings.value("Password","").toString());

    if (db.open())
    {
        MainWindow w;

        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::warning(0,"Connexion","Unable to connect to nanoTSAdmin database",QMessageBox::Ok);
        settings.remove("ID");
        return 1;
    }
}

