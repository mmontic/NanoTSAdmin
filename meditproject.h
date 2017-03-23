#ifndef MEDITPROJECT_H
#define MEDITPROJECT_H

#include <QDialog>

namespace Ui {
class MEditProject;
}

class MEditProject : public QDialog
{
    Q_OBJECT

public:
    explicit MEditProject(QWidget *parent = 0);
    ~MEditProject();

    void initFields();

    bool editMode;
    bool result;

    QString projectID;
    QString projectName;
    QString projectPriority;
    QString projectStatus;
    QString projectType;

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::MEditProject *ui;
};

#endif // MEDITPROJECT_H
