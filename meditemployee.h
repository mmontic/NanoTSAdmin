#ifndef MEDITEMPLOYEE_H
#define MEDITEMPLOYEE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class MEditEmployee;
}

class MEditEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit MEditEmployee(QWidget *parent = 0);
    ~MEditEmployee();

    void initFields();

    bool editMode;
    bool result;

    QString employeeID;
    QString employeeName;
    QDate employeeStart;
    QDate employeeStop;
    QString employeeCalendarID;

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::MEditEmployee *ui;
};

#endif // MEDITEMPLOYEE_H
