#include <QMessageBox>
#include <QtDebug>

#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QComboBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "meditproject.h"
#include "meditemployee.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupModels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupModels()
{
    tsModel = new QSqlQueryModel;
    tsModel->setQuery("SELECT ts.TimeSheetID, ts.TimeSheetStart, ts.TimeSheetStop, em.EmployeeName, ca.CalendarName, pr.ProjectName, ts.TimeSheetComment FROM TimeSheets ts inner join Projects pr on pr.ProjectID=ts.Projects_ProjectID inner join Employees em on em.EmployeeID=ts.Employees_EmployeeID inner join Calendars ca on ca.CalendarID=em.Calendars_CalendarID ORDER BY ts.TimeSheetStart, ts.TimeSheetStop, em.EmployeeName;");
    tsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tsModel->setHeaderData(1, Qt::Horizontal, tr("Start"));
    tsModel->setHeaderData(2, Qt::Horizontal, tr("Stop"));
    tsModel->setHeaderData(3, Qt::Horizontal, tr("Employee"));
    tsModel->setHeaderData(4, Qt::Horizontal, tr("Calendar"));
    tsModel->setHeaderData(5, Qt::Horizontal, tr("Project"));
    tsModel->setHeaderData(6, Qt::Horizontal, tr("Comment"));

    ui->tblTimeSheets->setModel(tsModel);
    ui->tblTimeSheets->setColumnWidth(0,0);
    ui->tblTimeSheets->setColumnWidth(1,105);
    ui->tblTimeSheets->setColumnWidth(2,105);
    ui->tblTimeSheets->setColumnWidth(3,200);
    ui->tblTimeSheets->setColumnWidth(4,200);
    ui->tblTimeSheets->setColumnWidth(5,200);
    ui->tblTimeSheets->setColumnWidth(6,250);

    projectModel = new QSqlQueryModel;
    projectModel->setQuery("SELECT pr.ProjectID, pr.ProjectName, pr.ProjectPriority, pr.ProjectStatus, pr.ProjectType FROM Projects pr ORDER BY pr.ProjectName");
    projectModel->setHeaderData(0, Qt::Horizontal, tr("Project ID"));
    projectModel->setHeaderData(1, Qt::Horizontal, tr("Project name"));
    projectModel->setHeaderData(2, Qt::Horizontal, tr("Project priority"));
    projectModel->setHeaderData(3, Qt::Horizontal, tr("Project status"));
    projectModel->setHeaderData(4, Qt::Horizontal, tr("Project type"));

    ui->tblProjects->setModel(projectModel);
    ui->tblProjects->setColumnWidth(0,0);
    ui->tblProjects->setColumnWidth(1,300);
    ui->tblProjects->setColumnWidth(2,125);
    ui->tblProjects->setColumnWidth(3,125);
    ui->tblProjects->setColumnWidth(4,125);

    employeeModel = new QSqlQueryModel;
    employeeModel->setQuery("SELECT em.EmployeeID, em.EmployeeName, EmployeeStart, EmployeeStop, ca.CalendarName FROM Employees em INNER JOIN Calendars ca ON ca.CalendarID = em.Calendars_CalendarID ORDER BY em.EmployeeName");
    employeeModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    employeeModel->setHeaderData(1, Qt::Horizontal, tr("Employee name"));
    employeeModel->setHeaderData(2, Qt::Horizontal, tr("Entry date"));
    employeeModel->setHeaderData(3, Qt::Horizontal, tr("Leave date"));
    employeeModel->setHeaderData(4, Qt::Horizontal, tr("Calendar"));

    ui->tblEmployees->setModel(employeeModel);
    ui->tblEmployees->setColumnWidth(0,0);
    ui->tblEmployees->setColumnWidth(1,275);
    ui->tblEmployees->setColumnWidth(4,275);

    calendarHeadsModel = new QSqlQueryModel;
    calendarHeadsModel->setQuery("SELECT ca.CalendarID, ca.CalendarName FROM Calendars ca ORDER BY ca.CalendarName");
    calendarHeadsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    calendarHeadsModel->setHeaderData(1, Qt::Horizontal, tr("Calendar name"));

    ui->tblCalendarHeads->setModel(calendarHeadsModel);
    ui->tblCalendarHeads->setColumnWidth(0,275);
    ui->tblCalendarHeads->setColumnWidth(1,275);

    calendarLinesModel = new QSqlQueryModel;
    calendarLinesModel->setQuery("SELECT ca.CalendarDetailDate, ca.CalendarDetailType FROM CalendarDetails ca ORDER BY ca.CalendarDetailDate");
    calendarLinesModel->setHeaderData(0, Qt::Horizontal, tr("Date"));
    calendarLinesModel->setHeaderData(1, Qt::Horizontal, tr("Date type"));

    ui->tblCalendarLines->setModel(calendarLinesModel);
    ui->tblCalendarLines->setColumnWidth(0,105);
    ui->tblCalendarLines->setColumnWidth(1,105);
}

void MainWindow::on_addTimeSheet_clicked()
{

}

void MainWindow::on_addProject_clicked()
{
    MEditProject *editor;

    editor = new MEditProject();
    editor->editMode = false;
    if (editor->exec())
    {
        QSqlQuery insQry("");

        if (insQry.prepare("INSERT INTO Projects (ProjectID, ProjectName, ProjectPriority, ProjectStatus, ProjectType) "
                       "VALUES (:ProjectID, :ProjectName, :ProjectPriority, :ProjectStatus, :ProjectType);"))
        {
            insQry.bindValue(":ProjectID", QUuid::createUuid().toString());
            insQry.bindValue(":ProjectName", editor->projectName);
            insQry.bindValue(":ProjectPriority", editor->projectPriority);
            insQry.bindValue(":ProjectStatus", editor->projectStatus);
            insQry.bindValue(":ProjectType", editor->projectType);
            insQry.exec();
            if (!insQry.isActive())
                qDebug() << "Project insert failure " << insQry.lastError();
            else
                projectModel->setQuery("SELECT pr.ProjectID, pr.ProjectName, pr.ProjectPriority, pr.ProjectStatus, pr.ProjectType FROM Projects pr ORDER BY pr.ProjectName");
        }
    }
}

void MainWindow::on_editProject_clicked()
{
    int rowidx = ui->tblProjects->selectionModel()->currentIndex().row();
    QString projectID = projectModel->index(rowidx , 0).data().toString();

    MEditProject *editor;

    editor = new MEditProject();
    editor->editMode = true;

    editor->projectID = projectID;
    editor->projectName = projectModel->index(rowidx , 1).data().toString();
    editor->projectPriority = projectModel->index(rowidx , 2).data().toString();
    editor->projectStatus = projectModel->index(rowidx , 3).data().toString();
    editor->projectType = projectModel->index(rowidx , 4).data().toString();
    editor->initFields();

    if (editor->exec())
    {
        QSqlQuery updQry("");

        if (updQry.prepare("UPDATE Projects SET ProjectName=:ProjectName, ProjectPriority=:ProjectPriority, ProjectStatus=:ProjectStatus, "
                       "ProjectType=:ProjectType WHERE ProjectID='" + projectID + "'"))
        {
            updQry.bindValue(":ProjectName", editor->projectName);
            updQry.bindValue(":ProjectPriority", editor->projectPriority);
            updQry.bindValue(":ProjectStatus", editor->projectStatus);
            updQry.bindValue(":ProjectType", editor->projectType);
            updQry.exec();
            if (!updQry.isActive())
                qDebug() << "Project update failure " << updQry.lastError();
            else
                projectModel->setQuery("SELECT pr.ProjectID, pr.ProjectName, pr.ProjectPriority, pr.ProjectStatus, pr.ProjectType FROM Projects pr ORDER BY pr.ProjectName");
        }
    }
}

void MainWindow::on_deleteProject_clicked()
{
    int rowidx = ui->tblProjects->selectionModel()->currentIndex().row();
    QString projectID = projectModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM Projects WHERE ProjectID='" + projectID + "'"))
        QMessageBox::warning(0,"Projects","Unable to delete project " + projectModel->index(rowidx , 1).data().toString());
    else
        projectModel->setQuery("SELECT pr.ProjectID, pr.ProjectName, pr.ProjectPriority, pr.ProjectStatus, pr.ProjectType FROM Projects pr ORDER BY pr.ProjectName");
}

void MainWindow::on_addEmployee_clicked()
{
    MEditEmployee *editor;

    editor = new MEditEmployee();
    editor->editMode = false;
    if (editor->exec())
    {
        QSqlQuery insQry("");

        if (insQry.prepare("INSERT INTO Employees (EmployeeID, EmployeeName, EmployeeStart, EmployeeStop, Calendars_CalendarID) "
                       "VALUES (:EmployeeID, :EmployeeName, :EmployeeStart, :EmployeeStop, :EmployeeCalendarID);"))
        {
            insQry.bindValue(":EmployeeID", QUuid::createUuid().toString());
            insQry.bindValue(":EmployeeName", editor->employeeName);
            insQry.bindValue(":EmployeeStart", editor->employeeStart);
            insQry.bindValue(":EmployeeStop", editor->employeeStop);
            insQry.bindValue(":EmployeeCalendarID",editor->employeeCalendarID);
            insQry.exec();
            if (!insQry.isActive())
                qDebug() << "Employee insert failure " << insQry.lastError();
            else
                employeeModel->setQuery("SELECT em.EmployeeID, em.EmployeeName, EmployeeStart, EmployeeStop, ca.CalendarName FROM Employees em INNER JOIN Calendars ca ON ca.CalendarID = em.Calendars_CalendarID ORDER BY em.EmployeeName");
        }
    }
}

void MainWindow::on_editEmployee_clicked()
{
    int rowidx = ui->tblEmployees->selectionModel()->currentIndex().row();
    QString employeeID = employeeModel->index(rowidx , 0).data().toString();

    MEditEmployee *editor;

    editor = new MEditEmployee();
    editor->editMode = true;

    editor->employeeID = employeeID;
    editor->employeeName = projectModel->index(rowidx , 1).data().toString();
    editor->employeeStart = projectModel->index(rowidx , 2).data().toDate();
    editor->employeeStop = projectModel->index(rowidx , 3).data().toDate();

    editor->initFields();

    if (editor->exec())
    {
        QSqlQuery updQry("");

        if (updQry.prepare("UPDATE Employees SET EmployeeName=:EmployeeName, EmployeeStart=:EmployeeStart, EmployeeStop=:EmployeeStop, "
                       "EmployeeCalendarID=:EmployeeCalendarID WHERE EmployeeID='" + employeeID + "'"))
        {
            updQry.bindValue(":EmployeeName", editor->employeeName);
            updQry.bindValue(":EmployeeStart", editor->employeeStart);
            updQry.bindValue(":EmployeeStop", editor->employeeStop);
            updQry.bindValue(":EmployeeCalendarID",editor->employeeCalendarID);
            updQry.exec();
            if (!updQry.isActive())
                qDebug() << "Employee update failure " << updQry.lastError();
            else
                employeeModel->setQuery("SELECT em.EmployeeID, em.EmployeeName, EmployeeStart, EmployeeStop, ca.CalendarName FROM Employees em INNER JOIN Calendars ca ON ca.CalendarID = em.Calendars_CalendarID ORDER BY em.EmployeeName");
        }
    }
}

void MainWindow::on_deleteEmployee_clicked()
{
    int rowidx = ui->tblEmployees->selectionModel()->currentIndex().row();
    QString employeeID = employeeModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM Employees WHERE EmployeeID='" + employeeID + "'"))
        QMessageBox::warning(0,"Employees","Unable to delete employee " + employeeModel->index(rowidx , 1).data().toString());
    else
        employeeModel->setQuery("SELECT em.EmployeeID, em.EmployeeName, EmployeeStart, EmployeeStop, ca.CalendarName FROM Employees em INNER JOIN Calendars ca ON ca.CalendarID = em.Calendars_CalendarID ORDER BY em.EmployeeName");
}
