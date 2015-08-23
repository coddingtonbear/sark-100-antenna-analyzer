#include <QString>
#include <QSettings>

#include "config.h"

namespace Config
{
  const char
    *Org = "Jedi98",
    *App = "Antenna Analyzer",
    *DOM_ENCODING = "UTF-8";

  QString dir_data;
  double swr_max, swr_bw_max, Z_Target;

  void read()
  {
    QSettings settings(Org, App);

    dir_data = settings.value("dir_data",".").toString();
    swr_max = settings.value("swr_max","10").toDouble();
    swr_bw_max = settings.value("swr_bw_max","1.5").toDouble();
    Z_Target = settings.value("Z_Target","50").toDouble();
  }

  void write()
  {
    QSettings settings(Org, App);

    settings.setValue("dir_data", dir_data);
    settings.setValue("swr_max", swr_max);
    settings.setValue("swr_bw_max", swr_bw_max);
    settings.setValue("Z_Target", Z_Target);
  }
}
