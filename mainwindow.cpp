#include <QMessageBox>
#include <QtDebug>

#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QComboBox>
#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "meditproject.h"
#include "meditemployee.h"
#include "meditsettings.h"
#include "medittimesheet.h"
#include "meditcalendarhead.h"
#include "meditcalendarline.h"

//************************************************************************************************
// MainWindow constructor
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Initialize the user interface and setup the models
//************************************************************************************************

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupModels();
}

//************************************************************************************************
// MainWindow destructor
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Remove the user interface (window)
//************************************************************************************************

MainWindow::~MainWindow()
{
    delete ui;
}

//************************************************************************************************
// MainWindow edit settings
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Invoque the settings editor to edit the database connection parameters
//************************************************************************************************

void MainWindow::editSettings()
{
    MEditSettings editor;
    editor.exec();
}

//************************************************************************************************
// MainWindow updateProjectView
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Reset the query used by the projectModel to refresh the data in the grid
//************************************************************************************************

void MainWindow::updateProjectView()
{
    projectModel->setQuery("SELECT pr.ProjectID, pr.ProjectName, pr.ProjectPriority, pr.ProjectStatus, pr.ProjectType "
                           "FROM Projects pr "
                           "ORDER BY pr.ProjectName");
}

//************************************************************************************************
// MainWindow updateEmployeeView
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Reset the query used by the employeeModel to refresh the data in the grid
//************************************************************************************************

void MainWindow::updateEmployeeView()
{
    employeeModel->setQuery("SELECT em.EmployeeID, em.EmployeeName, em.EmployeeStart, em.EmployeeStop, ca.CalendarName, em.Calendars_CalendarID "
                            "FROM Employees em INNER JOIN Calendars ca ON ca.CalendarID = em.Calendars_CalendarID "
                            "ORDER BY em.EmployeeName");
}

//************************************************************************************************
// MainWindow updateCalendarLinesView
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Reset the query used by the calendarLinesModel to refresh the data in the grid
//************************************************************************************************

void MainWindow::updateCalendarLinesView()
{
    int rowidx = ui->tblCalendarHeads->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString calendarID = calendarHeadsModel->index(rowidx , 0).data().toString();

    calendarLinesModel->setQuery("SELECT ca.CalendarDetailID, ca.Calendars_calendarID, ca.CalendarDetailDate, ca.CalendarDetailType "
                                 "FROM CalendarDetails ca "
                                 "WHERE Calendars_CalendarID = '" + calendarID + "'"
                                 "ORDER BY ca.CalendarDetailDate");

    calendarLinesModel->setHeaderData(0, Qt::Horizontal, tr("Detail ID"));
    calendarLinesModel->setHeaderData(1, Qt::Horizontal, tr("Calendar ID"));
    calendarLinesModel->setHeaderData(2, Qt::Horizontal, tr("Date"));
    calendarLinesModel->setHeaderData(3, Qt::Horizontal, tr("Date type"));

    ui->tblCalendarLines->setModel(calendarLinesModel);
    ui->tblCalendarLines->setColumnWidth(0,0);
    ui->tblCalendarLines->setColumnWidth(1,0);
    ui->tblCalendarLines->setColumnWidth(2,105);
    ui->tblCalendarLines->setColumnWidth(3,105);
}

//************************************************************************************************
// MainWindow updateCalendarHeadsView
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Reset the query used by the calendarHeadsModel to refresh the data in the grid
//************************************************************************************************

void MainWindow::updateCalendarHeadsView()
{
    calendarHeadsModel->setQuery("SELECT ca.CalendarID, ca.CalendarName "
                                 "FROM Calendars ca "
                                 "ORDER BY ca.CalendarName");
}

//************************************************************************************************
// MainWindow updateTimeSheetsView
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Reset the query used by the tsModel to refresh the data in the grid
//************************************************************************************************

void MainWindow::updateTimeSheetsView()
{
    tsModel->setQuery("SELECT ts.TimeSheetID, ts.Employees_EmployeeID, ts.Projects_ProjectID, ts.TimeSheetStart, ts.TimeSheetStop, em.EmployeeName, ca.CalendarName, pr.ProjectName, ts.TimeSheetComment "
                      "FROM TimeSheets ts "
                      "inner join Projects pr on pr.ProjectID=ts.Projects_ProjectID "
                      "inner join Employees em on em.EmployeeID=ts.Employees_EmployeeID "
                      "inner join Calendars ca on ca.CalendarID=em.Calendars_CalendarID "
                      "ORDER BY ts.TimeSheetStart, ts.TimeSheetStop, em.EmployeeName;");
}

//************************************************************************************************
// MainWindow setupModels
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Setup the models used by the grids in the user interface
//************************************************************************************************

void MainWindow::setupModels()
{
    tsModel = new QSqlQueryModel;
    updateTimeSheetsView();
    tsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tsModel->setHeaderData(1, Qt::Horizontal, tr("Employee ID"));
    tsModel->setHeaderData(2, Qt::Horizontal, tr("Project ID"));
    tsModel->setHeaderData(3, Qt::Horizontal, tr("Start"));
    tsModel->setHeaderData(4, Qt::Horizontal, tr("Stop"));
    tsModel->setHeaderData(5, Qt::Horizontal, tr("Employee"));
    tsModel->setHeaderData(6, Qt::Horizontal, tr("Calendar"));
    tsModel->setHeaderData(7, Qt::Horizontal, tr("Project"));
    tsModel->setHeaderData(8, Qt::Horizontal, tr("Comment"));

    ui->tblTimeSheets->setModel(tsModel);
    ui->tblTimeSheets->setColumnWidth(0,0);
    ui->tblTimeSheets->setColumnWidth(1,0);
    ui->tblTimeSheets->setColumnWidth(2,0);
    ui->tblTimeSheets->setColumnWidth(3,105);
    ui->tblTimeSheets->setColumnWidth(4,105);
    ui->tblTimeSheets->setColumnWidth(5,200);
    ui->tblTimeSheets->setColumnWidth(6,200);
    ui->tblTimeSheets->setColumnWidth(7,200);
    ui->tblTimeSheets->setColumnWidth(8,250);

    projectModel = new QSqlQueryModel;
    updateProjectView();
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
    updateEmployeeView();
    employeeModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    employeeModel->setHeaderData(1, Qt::Horizontal, tr("Employee name"));
    employeeModel->setHeaderData(2, Qt::Horizontal, tr("Entry date"));
    employeeModel->setHeaderData(3, Qt::Horizontal, tr("Leave date"));
    employeeModel->setHeaderData(4, Qt::Horizontal, tr("Calendar"));
    employeeModel->setHeaderData(4, Qt::Horizontal, tr("Calendar ID"));

    ui->tblEmployees->setModel(employeeModel);
    ui->tblEmployees->setColumnWidth(0,0);
    ui->tblEmployees->setColumnWidth(1,300);
    ui->tblEmployees->setColumnWidth(2,100);
    ui->tblEmployees->setColumnWidth(3,100);
    ui->tblEmployees->setColumnWidth(4,300);
    ui->tblEmployees->setColumnWidth(5,0);

    calendarHeadsModel = new QSqlQueryModel;
    updateCalendarHeadsView();
    calendarHeadsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    calendarHeadsModel->setHeaderData(1, Qt::Horizontal, tr("Calendar name"));

    ui->tblCalendarHeads->setModel(calendarHeadsModel);
    ui->tblCalendarHeads->setColumnWidth(0,0);
    ui->tblCalendarHeads->setColumnWidth(1,275);

    calendarLinesModel = new QSqlQueryModel;
    updateCalendarLinesView();
}


//************************************************************************************************
// MainWindow on_addTimeSheet_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Add a new time sheet entry
//************************************************************************************************

void MainWindow::on_addTimeSheet_clicked()
{
    MEditTimeSheet *editor;

    editor = new MEditTimeSheet();
    editor->editMode = false;
    if (editor->exec())
    {
        QSqlQuery insQry("");

        if (insQry.prepare("INSERT INTO TimeSheets (TimeSheetID, TimeSheetStart, TimeSheetStop, TimeSheetComment, Projects_ProjectID, Employees_EmployeeID) "
                           "VALUES (:TimeSheetID, :TimeSheetStart, :TimeSheetStop, :TimeSheetComment, :ProjectID, :EmployeeID);"))
        {
            insQry.bindValue(":TimeSheetID", QUuid::createUuid().toString());
            insQry.bindValue(":TimeSheetStart", editor->timesheetStart);
            insQry.bindValue(":TimeSheetStop", editor->timesheetStop);
            insQry.bindValue(":TimeSheetComment", editor->timesheetComment);
            insQry.bindValue(":ProjectID", editor->projectID);
            insQry.bindValue(":EmployeeID", editor->employeeID);
            insQry.exec();
            if (!insQry.isActive())
                qDebug() << "Time sheet insert failure " << insQry.lastError();
            else
                updateTimeSheetsView();
        }
    }
}

//************************************************************************************************
// MainWindow on_tblProjects_doubleClicked
//************************************************************************************************
// Params : const QModelIndex &index
//************************************************************************************************
// Return : void
//************************************************************************************************
// Call the edit method when double click in the timesheet grid
//************************************************************************************************

void MainWindow::on_tblTimeSheets_doubleClicked(const QModelIndex &index)
{
    on_editTimeSheet_clicked();
}

//************************************************************************************************
// MainWindow on_editTimeSheet_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Edit a time sheet entry
//************************************************************************************************

void MainWindow::on_editTimeSheet_clicked()
{
    int rowidx = ui->tblTimeSheets->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString timesheetID = tsModel->index(rowidx , 0).data().toString();

    MEditTimeSheet *editor;

    editor = new MEditTimeSheet();
    editor->editMode = true;

    editor->timesheetID = timesheetID;
    editor->employeeID = tsModel->index(rowidx , 1).data().toString();
    editor->projectID = tsModel->index(rowidx , 2).data().toString();
    editor->timesheetStart = tsModel->index(rowidx , 3).data().toDateTime();
    editor->timesheetStop = tsModel->index(rowidx , 4).data().toDateTime();
    editor->timesheetComment = tsModel->index(rowidx , 8).data().toString();
    editor->initFields();

    if (editor->exec())
    {
        QSqlQuery updQry("");

        if (updQry.prepare("UPDATE TimeSheets SET TimeSheetStart=:TimeSheetStart, TimeSheetStop=:TimeSheetStop, TimeSheetComment=:TimeSheetComment, "
                           "Projects_ProjectID=:ProjectID, Employees_EmployeeID=:EmployeeID WHERE TimeSheetID='" + timesheetID + "'"))
        {
            updQry.bindValue(":TimeSheetStart", editor->timesheetStart);
            updQry.bindValue(":TimeSheetStop", editor->timesheetStop);
            updQry.bindValue(":TimeSheetComment", editor->timesheetComment);
            updQry.bindValue(":ProjectID", editor->projectID);
            updQry.bindValue(":EmployeeID", editor->employeeID);
            updQry.exec();
            if (!updQry.isActive())
                qDebug() << "Time sheet update failure " << updQry.lastError();
            else
                updateTimeSheetsView();
        }
    }
}

//************************************************************************************************
// MainWindow on_deleteTimeSheet_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Delete a time sheet entry
//************************************************************************************************

void MainWindow::on_deleteTimeSheet_clicked()
{
    int rowidx = ui->tblTimeSheets->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString timesheetID = tsModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM TimeSheets WHERE TimeSheetID='" + timesheetID + "'"))
        QMessageBox::warning(0,"Time sheets","Unable to delete time sheet " + tsModel->index(rowidx , 1).data().toString());
    else
        updateTimeSheetsView();
}

//************************************************************************************************
// MainWindow on_addProject_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Add a new project
//************************************************************************************************

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
                updateProjectView();
        }
    }
}

//************************************************************************************************
// MainWindow on_tblProjects_doubleClicked
//************************************************************************************************
// Params : const QModelIndex &index
//************************************************************************************************
// Return : void
//************************************************************************************************
// Call the edit method when double click in the project grid
//************************************************************************************************

void MainWindow::on_tblProjects_doubleClicked(const QModelIndex &index)
{
    on_editProject_clicked();
}

//************************************************************************************************
// MainWindow on_editProject_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Edit a project
//************************************************************************************************

void MainWindow::on_editProject_clicked()
{
    int rowidx = ui->tblProjects->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

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
                updateProjectView();
        }
    }
}

//************************************************************************************************
// MainWindow on_deleteProject_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Delete a project
//************************************************************************************************

void MainWindow::on_deleteProject_clicked()
{
    int rowidx = ui->tblProjects->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString projectID = projectModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM Projects WHERE ProjectID='" + projectID + "'"))
        QMessageBox::warning(0,"Projects","Unable to delete project " + projectModel->index(rowidx , 1).data().toString());
    else
        updateProjectView();
}

//************************************************************************************************
// MainWindow on_addEmployee_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Add a new employee
//************************************************************************************************

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
                updateEmployeeView();
        }
    }
}

//************************************************************************************************
// MainWindow on_tblEmployees_doubleClicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Call the edit method when double click in the employee grid
//************************************************************************************************

void MainWindow::on_tblEmployees_doubleClicked(const QModelIndex &index)
{
    on_editEmployee_clicked();
}

//************************************************************************************************
// MainWindow on_editEmployee_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Edit an employee
//************************************************************************************************

void MainWindow::on_editEmployee_clicked()
{
    int rowidx = ui->tblEmployees->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString employeeID = employeeModel->index(rowidx , 0).data().toString();

    MEditEmployee *editor;

    editor = new MEditEmployee();
    editor->editMode = true;

    editor->employeeID = employeeID;
    editor->employeeName = employeeModel->index(rowidx , 1).data().toString();
    editor->employeeStart = employeeModel->index(rowidx , 2).data().toDate();
    editor->employeeStop = employeeModel->index(rowidx , 3).data().toDate();
    editor->employeeCalendarID = employeeModel->index(rowidx , 5).data().toString();

    editor->initFields();

    if (editor->exec())
    {
        QSqlQuery updQry("");

        if (updQry.prepare("UPDATE Employees SET EmployeeName=:EmployeeName, EmployeeStart=:EmployeeStart, EmployeeStop=:EmployeeStop, "
                           "Calendars_CalendarID=:EmployeeCalendarID WHERE EmployeeID='" + employeeID + "'"))
        {
            updQry.bindValue(":EmployeeName", editor->employeeName);
            updQry.bindValue(":EmployeeStart", editor->employeeStart);
            updQry.bindValue(":EmployeeStop", editor->employeeStop);
            updQry.bindValue(":EmployeeCalendarID",editor->employeeCalendarID);
            updQry.exec();
            if (!updQry.isActive())
                qDebug() << "Employee update failure " << updQry.lastError();
            else
                updateEmployeeView();
        }
    }
}

//************************************************************************************************
// MainWindow on_deleteEmployee_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Delete an employee
//************************************************************************************************

void MainWindow::on_deleteEmployee_clicked()
{
    int rowidx = ui->tblEmployees->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString employeeID = employeeModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM Employees WHERE EmployeeID='" + employeeID + "'"))
        QMessageBox::warning(0,"Employees","Unable to delete employee " + employeeModel->index(rowidx , 1).data().toString());
    else
        updateEmployeeView();
}

//************************************************************************************************
// MainWindow on_addCalendarHead_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Add a calendar entry
//************************************************************************************************

void MainWindow::on_addCalendarHead_clicked()
{
    MEditCalendarHead *editor;

    editor = new MEditCalendarHead();
    editor->editMode = false;
    if (editor->exec())
    {
        QSqlQuery insQry("");

        if (insQry.prepare("INSERT INTO Calendars (CalendarID, CalendarName) "
                           "VALUES (:CalendarID, :CalendarName);"))
        {
            insQry.bindValue(":CalendarID", QUuid::createUuid().toString());
            insQry.bindValue(":CalendarName", editor->calendarName);
            insQry.exec();
            if (!insQry.isActive())
                qDebug() << "Calendar insert failure " << insQry.lastError();
            else
                updateCalendarHeadsView();
        }
    }
}

//************************************************************************************************
// MainWindow on_tblCalendarHeads_doubleClicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Call the edit method when double click in the calendar head grid
//************************************************************************************************

void MainWindow::on_tblCalendarHeads_doubleClicked(const QModelIndex &index)
{
    on_editCalendarHead_clicked();
}

//************************************************************************************************
// MainWindow on_editCalendarHead_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Edit a calendar entry
//************************************************************************************************

void MainWindow::on_editCalendarHead_clicked()
{
    int rowidx = ui->tblCalendarHeads->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString calendarID = calendarHeadsModel->index(rowidx , 0).data().toString();

    MEditCalendarHead *editor;

    editor = new MEditCalendarHead();
    editor->editMode = true;

    editor->calendarID = calendarID;
    editor->calendarName = calendarHeadsModel->index(rowidx , 1).data().toString();

    editor->initFields();

    if (editor->exec())
    {
        QSqlQuery updQry("");

        if (updQry.prepare("UPDATE Calendars SET CalendarName=:CalendarName "
                           "WHERE CalendarID='" + calendarID + "'"))
        {
            updQry.bindValue(":CalendarName", editor->calendarName);
            updQry.exec();
            if (!updQry.isActive())
                qDebug() << "Calendar head update failure " << updQry.lastError();
            else
                updateCalendarHeadsView();
        }
    }
}

//************************************************************************************************
// MainWindow on_deleteCalendarHead_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Delete a calendar entry
//************************************************************************************************

void MainWindow::on_deleteCalendarHead_clicked()
{
    int rowidx = ui->tblCalendarHeads->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString calendarID = calendarHeadsModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM Calendars WHERE CalendarID='" + calendarID + "'"))
        QMessageBox::warning(0,"Calendars","Unable to delete calendar " + calendarHeadsModel->index(rowidx , 1).data().toString());
    else
        updateCalendarHeadsView();
}

//************************************************************************************************
// MainWindow on_addCalendarLine_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Add a calendar line
//************************************************************************************************

void MainWindow::on_addCalendarLine_clicked()
{
    MEditCalendarLine *editor;

    editor = new MEditCalendarLine();
    editor->editMode = false;
    if (editor->exec())
    {
        int rowidx = ui->tblCalendarHeads->selectionModel()->currentIndex().row();
        if (rowidx == -1) return;

        QString calendarID = calendarHeadsModel->index(rowidx , 0).data().toString();

        QSqlQuery insQry("");

        if (insQry.prepare("INSERT INTO CalendarDetails (CalendarDetailID, CalendarDetailDate, CalendarDetailType, Calendars_CalendarID) "
                           "VALUES (:CalendarDetailID, :CalendarDetailDate, :CalendarDetailType, :CalendarID);"))
        {
            insQry.bindValue(":CalendarDetailID", QUuid::createUuid().toString());
            insQry.bindValue(":CalendarDetailDate", editor->calendarDetailDate);
            insQry.bindValue(":CalendarDetailType", editor->calendarDetailType);
            insQry.bindValue(":CalendarID", calendarID);
            insQry.exec();
            if (!insQry.isActive())
                qDebug() << "Calendar insert failure " << insQry.lastError();
            else
                updateCalendarLinesView();
        }
    }
}

//************************************************************************************************
// MainWindow on_tblCalendarHeads_doubleClicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Call the edit method when double click in the calendar head grid
//************************************************************************************************

void MainWindow::on_tblCalendarLines_doubleClicked(const QModelIndex &index)
{
    on_editCalendarLine_clicked();
}

//************************************************************************************************
// MainWindow on_editCalendarLine_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Edit a calendar line
//************************************************************************************************

void MainWindow::on_editCalendarLine_clicked()
{
    int rowidx = ui->tblCalendarLines->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString calendarDetailID = calendarLinesModel->index(rowidx , 0).data().toString();

    MEditCalendarLine *editor;

    editor = new MEditCalendarLine();
    editor->editMode = true;

    editor->calendarDetailID = calendarDetailID;
    editor->calendarDetailDate = calendarLinesModel->index(rowidx , 2).data().toDate();
    editor->calendarDetailType = calendarLinesModel->index(rowidx , 3).data().toString();

    editor->initFields();

    if (editor->exec())
    {
        QSqlQuery updQry("");

        if (updQry.prepare("UPDATE CalendarDetails SET CalendarDetailDate=:CalendarDetailDate, CalendarDetailType=:CalendarDetailType "
                           "WHERE CalendarDetailID='" + calendarDetailID + "'"))
        {
            updQry.bindValue(":CalendarDetailDate", editor->calendarDetailDate);
            updQry.bindValue(":CalendarDetailType", editor->calendarDetailType);
            updQry.exec();
            if (!updQry.isActive())
                qDebug() << "Calendar line update failure " << updQry.lastError();
            else
                updateCalendarLinesView();
        }
    }
}

//************************************************************************************************
// MainWindow on_deleteCalendarLine_clicked
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// delete a calendar line
//************************************************************************************************

void MainWindow::on_deleteCalendarLine_clicked()
{
    int rowidx = ui->tblCalendarLines->selectionModel()->currentIndex().row();
    if (rowidx == -1) return;

    QString calendarDetailID = calendarLinesModel->index(rowidx , 0).data().toString();
    QSqlQuery delQry("");
    if (!delQry.exec("DELETE FROM CalendarDetails WHERE CalendarDetailID='" + calendarDetailID + "'"))
        QMessageBox::warning(0,"Calendar lines","Unable to delete calendar line " + calendarLinesModel->index(rowidx , 1).data().toString());
    else
        updateCalendarLinesView();
}

//************************************************************************************************
// MainWindow synchroCalendarLines
//************************************************************************************************
// Params : None
//************************************************************************************************
// Return : void
//************************************************************************************************
// Show the lines of a calendar based on the calendar head selection
//************************************************************************************************

void MainWindow::synchroCalendarLines()
{
    updateCalendarLinesView();
}
