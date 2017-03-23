#include <QSqlQuery>
#include <QMessageBox>

#include "meditproject.h"
#include "ui_meditproject.h"

MEditProject::MEditProject(QWidget *parent) : QDialog(parent), ui(new Ui::MEditProject)
{
    ui->setupUi(this);

    ui->projectPriority->addItem("LOW");
    ui->projectPriority->addItem("MEDIUM");
    ui->projectPriority->addItem("HIGH");

    ui->projectStatus->addItem("IDLE");
    ui->projectStatus->addItem("RUNNING");
    ui->projectStatus->addItem("CLOSED");

    ui->projectType->addItem("SUPPORT");
    ui->projectType->addItem("ROLLOUT");
    ui->projectType->addItem("ENRICHMENT");
}

MEditProject::~MEditProject()
{
    delete ui;
}

void MEditProject::initFields()
{
    int index ;

    ui->projectName->setText(projectName);
    index = ui->projectPriority->findText(projectPriority);
    if (index != -1)
        ui->projectPriority->setCurrentIndex(index);

    index = ui->projectStatus->findText(projectStatus);
    if (index != -1)
        ui->projectStatus->setCurrentIndex(index);

    index = ui->projectType->findText(projectType);
    if (index != -1)
        ui->projectType->setCurrentIndex(index);

    ui->projectName->setEnabled(false);
}

void MEditProject::on_okButton_clicked()
{
    if (!editMode)
    {
        bool ok;
        QString str = ui->projectName->text();
        QSqlQuery qryProject("");

        ok = qryProject.exec("SELECT ProjectID FROM Projects WHERE ProjectName='" + str + "'");
        if (ok && !qryProject.next())
        {
            projectName = ui->projectName->text();
            projectPriority = ui->projectPriority->currentText();
            projectStatus = ui->projectStatus->currentText();
            projectType = ui->projectType->currentText();

            result = true;
            accept();
        }
        else
            QMessageBox::warning(0,"Edition","This project name is already used",QMessageBox::Ok);
    }
    else
    {
        projectName = ui->projectName->text();
        projectPriority = ui->projectPriority->currentText();
        projectStatus = ui->projectStatus->currentText();
        projectType = ui->projectType->currentText();

        result = true;
        accept();
    }
}

void MEditProject::on_cancelButton_clicked()
{
    result = false;
    this->close();
}
