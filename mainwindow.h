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

    QSqlQueryModel *projectModel;
    QSqlQueryModel *employeeModel;
    QSqlQueryModel *tsModel;
    QSqlQueryModel *calendarHeadsModel;
    QSqlQueryModel *calendarLinesModel;

    void setupModels();

private slots:
    void on_addTimeSheet_clicked();
    void on_addProject_clicked();
    void on_deleteProject_clicked();
    void on_editProject_clicked();
    void on_addEmployee_clicked();
    void on_editEmployee_clicked();
    void on_deleteEmployee_clicked();
    void on_tblEmployees_doubleClicked(const QModelIndex &index);
    void on_tblProjects_doubleClicked(const QModelIndex &index);
    void on_addCalendarHead_clicked();
    void on_editCalendarHead_clicked();
    void on_deleteCalendarHead_clicked();
    void on_addCalendarLine_clicked();
    void on_editCalendarLine_clicked();
    void on_deleteCalendarLine_clicked();

    void editSettings();
    void synchroCalendarLines();

    void on_editTimeSheet_clicked();

    void on_deleteTimeSheet_clicked();

    void on_tblTimeSheets_doubleClicked(const QModelIndex &index);

    void on_tblCalendarHeads_doubleClicked(const QModelIndex &index);

    void on_tblCalendarLines_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;

    void updateTimeSheetsView();
    void updateProjectView();
    void updateEmployeeView();
    void updateCalendarHeadsView();
    void updateCalendarLinesView();
};

#endif // MAINWINDOW_H
