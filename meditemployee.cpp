#include <QVariant>
#include <QSqlQuery>
#include <QMessageBox>

#include "meditemployee.h"
#include "ui_meditemployee.h"

MEditEmployee::MEditEmployee(QWidget *parent) : QDialog(parent), ui(new Ui::MEditEmployee)
{
    ui->setupUi(this);    

    QSqlQuery qryCalendar("SELECT CalendarID, CalendarName from Calendars");
    while (qryCalendar.next())
    {
        QString calendarID = qryCalendar.value(0).toString();
        QString calendarName = qryCalendar.value(1).toString();
        ui->employeeCalendar->addItem(calendarName, calendarID);
    }
}

MEditEmployee::~MEditEmployee()
{
    delete ui;
}

void MEditEmployee::initFields()
{
    int index ;

    ui->employeeName->setText(employeeName);
    ui->employeeStart->setDate(employeeStart);
    ui->employeeStop->setDate(employeeStop);

    index = ui->employeeCalendar->findData(employeeCalendarID);
    if (index != -1)
        ui->employeeCalendar->setCurrentIndex(index);
}

void MEditEmployee::on_okButton_clicked()
{
    if (!editMode)
    {
        bool ok;
        QString str = ui->employeeName->text();
        QSqlQuery qryEmployee("");

        ok = qryEmployee.exec("SELECT EmployeeID FROM Employees WHERE EmployeeName='" + str + "'");
        if (ok && !qryEmployee.next())
        {
            employeeName = ui->employeeName->text();
            employeeStart = ui->employeeStart->date();
            employeeStop = ui->employeeStop->date();

            employeeCalendarID = ui->employeeCalendar->itemData(ui->employeeCalendar->currentIndex()).toString();

            result = true;
            accept();
        }
        else
            QMessageBox::warning(0,"Edition","This employee name is already used",QMessageBox::Ok);
    }
    else
    {
        employeeName = ui->employeeName->text();
        employeeStart = ui->employeeStart->date();
        employeeStop = ui->employeeStop->date();
        //employeeCalendarID = ui->projectType->currentText();  search ID of the selected calendar

        result = true;
        accept();
    }
}

void MEditEmployee::on_cancelButton_clicked()
{
    result = false;
    this->close();
}
