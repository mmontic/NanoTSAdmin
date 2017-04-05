#include "meditcalendarline.h"
#include "ui_meditcalendarline.h"

MEditCalendarLine::MEditCalendarLine(QWidget *parent) : QDialog(parent), ui(new Ui::MEditCalendarLine)
{
    ui->setupUi(this);

    QStringList calendarTypes;
    calendarTypes << tr("Ferie") << tr("Fete nationale") << tr("Jour offert");
    ui->calendarDetailType->addItems(calendarTypes);
}

MEditCalendarLine::~MEditCalendarLine()
{
    delete ui;
}

void MEditCalendarLine::initFields()
{
    ui->calendarDetailDate->setDate(calendarDetailDate);

    int index = ui->calendarDetailType->findText(calendarDetailType);
    if (index != -1)
        ui->calendarDetailType->setCurrentIndex(index);
}

void MEditCalendarLine::on_okButton_clicked()
{
    calendarDetailDate = ui->calendarDetailDate->date();
    calendarDetailType = ui->calendarDetailType->currentText();

    result = true;
    accept();
}

void MEditCalendarLine::on_cancelButton_clicked()
{
    result = false;
    this->close();
}
