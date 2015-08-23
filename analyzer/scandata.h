#ifndef SCANDATA_H
#define SCANDATA_H

#include <vector>

#include <dom.h>
#include "eventreceiver.h"

#define max3(v1,v2,v3) ((v1)>(v2)?((v1)>(v3)?(v1):(v3)):((v2)>(v3)?(v2):(v3)))

class Sample
{
public:
    Sample();
    void fromRaw(double vf,double vr,double vz,double va);

    double freq, swr, R, Z, X;
};

class ScanData
{
public:
    ScanData();
    void SetPointCount(int n);
    int GetPointCount();
    void UpdateStats();
    void dummy_data(EventReceiver *);
    void toDom(QDomDocument &doc,QDomElement &parent);
    bool fromDom(QDomElement &e0);

    std::vector<Sample> points;
    //int point_count;

    //timestamp??
    double freq_start,freq_end;
    int swr_min_idx, swr_max_idx, Z_min_idx, Z_max_idx, X_min_idx, X_max_idx, R_min_idx, R_max_idx;
    int swr_bw_lo_idx,swr_bw_hi_idx;
};

#endif // SCANDATA_H
