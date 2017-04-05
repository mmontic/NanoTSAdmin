#include <QVariant>
#include <QSqlQuery>
#include <QMessageBox>

#include "meditcalendarhead.h"
#include "ui_meditcalendarhead.h"

MEditCalendarHead::MEditCalendarHead(QWidget *parent) : QDialog(parent), ui(new Ui::MEditCalendarHead)
{
    ui->setupUi(this);
}

MEditCalendarHead::~MEditCalendarHead()
{
    delete ui;
}

void MEditCalendarHead::initFields()
{
    ui->calendarName->setText(calendarName);
}

void MEditCalendarHead::on_okButton_clicked()
{
    if (!editMode)
    {
        bool ok;
        QString str = ui->calendarName->text();
        QSqlQuery qryCalendar("");

        ok = qryCalendar.exec("SELECT CalendarID FROM Calendars WHERE CalendarName='" + str + "'");
        if (ok && !qryCalendar.next())
        {
            calendarName = ui->calendarName->text();

            result = true;
            accept();
        }
        else
            QMessageBox::warning(0,"Calendar edition","This calendar name is already used",QMessageBox::Ok);
    }
    else
    {
        calendarName = ui->calendarName->text();

        result = true;
        accept();
    }
}

void MEditCalendarHead::on_cancelButton_clicked()
{
    result = false;
    this->close();
}
