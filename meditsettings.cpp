#include <QSqlDatabase>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QUuid>

#include "meditsettings.h"
#include "ui_meditsettings.h"

MEditSettings::MEditSettings(QWidget *parent) : QDialog(parent), ui(new Ui::MEditSettings)
{
    ui->setupUi(this);
    QSettings settings;

    ui->hostName->setText(settings.value("HostName","").toString());
    ui->databaseName->setText(settings.value("DatabaseName","").toString());
    ui->userName->setText(settings.value("UserName","").toString());
    ui->password->setText(settings.value("Password","").toString());
}

MEditSettings::~MEditSettings()
{
    delete ui;
}

void MEditSettings::on_okButton_clicked()
{
    QSettings settings;

    if (settings.value("ID","") == "")
        settings.setValue("ID",QUuid::createUuid().toString());

    settings.setValue("HostName",ui->hostName->text());
    settings.setValue("DatabaseName",ui->databaseName->text());
    settings.setValue("UserName",ui->userName->text());
    settings.setValue("Password",ui->password->text());

    accept();
}

void MEditSettings::on_cancelButton_clicked()
{
    reject();
}

void MEditSettings::TestConnection()
{
    if (isConnectionOk())
        QMessageBox::information(0,tr("DB Connection"),tr("Successful connection"),QMessageBox::Ok);
    else
        QMessageBox::information(0,tr("DB Connection"),tr("Unable to connect to database\n"),QMessageBox::Ok);
}

void MEditSettings::updateTestConnection()
{
    ui->testConnection->setEnabled(((ui->hostName->text() != "") && (ui->databaseName->text() != "") && (ui->userName->text() != "")));
}

bool MEditSettings::isConnectionOk()
{
    bool result;
    QSqlDatabase TestDb = QSqlDatabase::addDatabase("QMYSQL","TESTDB");
    TestDb.setHostName(ui->hostName->text());
    TestDb.setDatabaseName(ui->databaseName->text());
    TestDb.setUserName(ui->userName->text());
    TestDb.setPassword(ui->password->text());

    result = TestDb.open();
    TestDb.close();

    return result;
}
