#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(QWidget *parent = 0);
    ~SettingsDlg();

private slots:
    void Slot_Accept();

private:
    Ui::SettingsDlg *ui;
};

#endif // SETTINGSDLG_H
