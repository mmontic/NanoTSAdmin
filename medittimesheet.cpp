#include <QSqlQuery>

#include "medittimesheet.h"
#include "ui_medittimesheet.h"

MEditTimeSheet::MEditTimeSheet(QWidget *parent) : QDialog(parent), ui(new Ui::MEditTimeSheet)
{
    ui->setupUi(this);

    QSqlQuery qryEmployee("SELECT EmployeeID, EmployeeName from Employees ORDER BY EmployeeName");
    while (qryEmployee.next())
    {
        QString employeeID = qryEmployee.value(0).toString();
        QString employeeName = qryEmployee.value(1).toString();
        ui->employee->addItem(employeeName, employeeID);
    }

    QSqlQuery qryProject("SELECT ProjectID, ProjectName from Projects ORDER BY ProjectName");
    while (qryProject.next())
    {
        QString projectID = qryProject.value(0).toString();
        QString projectName = qryProject.value(1).toString();
        ui->project->addItem(projectName, projectID);
    }
}

MEditTimeSheet::~MEditTimeSheet()
{
    delete ui;
}

void MEditTimeSheet::initFields()
{
    int index ;

    ui->timesheetStart->setDateTime(timesheetStart);
    ui->timesheetStop->setDateTime(timesheetStop);
    ui->timesheetComment->setText(timesheetComment);

    index = ui->employee->findData(employeeID);
    if (index != -1)
        ui->employee->setCurrentIndex(index);

    index = ui->project->findData(projectID);
    if (index != -1)
        ui->project->setCurrentIndex(index);
}

void MEditTimeSheet::on_okButton_clicked()
{
    timesheetStart = ui->timesheetStart->dateTime();
    timesheetStop = ui->timesheetStop->dateTime();
    timesheetComment = ui->timesheetComment->text();
    employeeID = ui->employee->itemData(ui->employee->currentIndex()).toString();
    projectID = ui->project->itemData(ui->project->currentIndex()).toString();

    result = true;
    accept();
}

void MEditTimeSheet::on_cancelButton_clicked()
{
    result = false;
    this->close();
}
