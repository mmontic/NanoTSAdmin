#ifndef MEDITCALENDARLINE_H
#define MEDITCALENDARLINE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class MEditCalendarLine;
}

class MEditCalendarLine : public QDialog
{
    Q_OBJECT

public:
    explicit MEditCalendarLine(QWidget *parent = 0);
    ~MEditCalendarLine();

    void initFields();

    bool editMode;
    bool result;

    QString calendarDetailID;
    QDate calendarDetailDate;
    QString calendarDetailType;

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::MEditCalendarLine *ui;
};

#endif // MEDITCALENDARLINE_H
