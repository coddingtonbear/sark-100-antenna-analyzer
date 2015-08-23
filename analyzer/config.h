#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

namespace Config
{
    extern QString dir_data;
    extern double swr_max, swr_bw_max, Z_Target;
    extern const char
      *Org,*App,*DOM_ENCODING;

    void read();
    void write();
}

#endif // CONFIG_H
