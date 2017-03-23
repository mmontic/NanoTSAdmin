#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QFileInfo>
#include <QSqlQuery>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//
//  Open the database connection
//

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("nanoTSAdmin");
    db.setUserName("root");


    if (db.open())
    {
        MainWindow w;

        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::warning(0,"Connexion","Unable to connecto to nanoTSAdmin database",QMessageBox::Ok);
        return 1;
    }
}

