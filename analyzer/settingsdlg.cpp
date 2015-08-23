#include "config.h"

#include "settingsdlg.h"
#include "ui_settingsdlg.h"

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(Slot_Accept()));

    ui->swr_max->setValue(Config::swr_bw_max);
    ui->swr_bw_max->setValue(Config::swr_bw_max);
    ui->Z_Target->setValue(Config::Z_Target);
}

void SettingsDlg::Slot_Accept()
{
    using namespace Config;

    swr_max = ui->swr_max->value();
    swr_bw_max = ui->swr_bw_max->value();
    Z_Target = ui->Z_Target->value();
    write();
}

SettingsDlg::~SettingsDlg()
{
    delete ui;
}
