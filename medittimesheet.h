#ifndef MEDITTIMESHEET_H
#define MEDITTIMESHEET_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class MEditTimeSheet;
}

class MEditTimeSheet : public QDialog
{
    Q_OBJECT

public:
    explicit MEditTimeSheet(QWidget *parent = 0);
    ~MEditTimeSheet();

    bool editMode;
    bool result;

    QString timesheetID;
    QDateTime timesheetStart;
    QDateTime timesheetStop;
    QString timesheetComment;
    QString projectID;
    QString employeeID;

    void initFields();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::MEditTimeSheet *ui;
};

#endif // MEDITTIMESHEET_H
