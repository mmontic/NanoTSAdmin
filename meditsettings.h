#ifndef MEDITSETTINGS_H
#define MEDITSETTINGS_H

#include <QDialog>

namespace Ui {
class MEditSettings;
}

class MEditSettings : public QDialog
{
    Q_OBJECT

public:
    explicit MEditSettings(QWidget *parent = 0);
    ~MEditSettings();

    bool isConnectionOk();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void TestConnection();
    void updateTestConnection();

private:
    Ui::MEditSettings *ui;
};

#endif // MEDITSETTINGS_H
