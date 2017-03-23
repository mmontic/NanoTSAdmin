#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtDebug>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QUuid>

#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupModels();
    QSqlQueryModel *projectModel;
    QSqlQueryModel *employeeModel;
    QSqlQueryModel *tsModel;
    QSqlQueryModel *calendarHeadsModel;
    QSqlQueryModel *calendarLinesModel;

private slots:
    void on_addTimeSheet_clicked();
    void on_addProject_clicked();

    void on_deleteProject_clicked();

    void on_editProject_clicked();

    void on_addEmployee_clicked();

    void on_editEmployee_clicked();

    void on_deleteEmployee_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // MAINWINDOW_H
