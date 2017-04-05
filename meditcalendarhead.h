#ifndef MEDITCALENDARHEAD_H
#define MEDITCALENDARHEAD_H

#include <QDialog>

namespace Ui {
class MEditCalendarHead;
}

class MEditCalendarHead : public QDialog
{
    Q_OBJECT

public:
    explicit MEditCalendarHead(QWidget *parent = 0);
    ~MEditCalendarHead();

    void initFields();

    bool editMode;
    bool result;

    QString calendarID;
    QString calendarName;

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::MEditCalendarHead *ui;
};

#endif // MEDITCALENDARHEAD_H
