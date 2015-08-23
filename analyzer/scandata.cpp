#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "config.h"

#include "eventreceiver.h"
#include "scandata.h"

Sample::Sample()
{
    freq = 0;
    swr = 1.0;
    Z = R = 50.0;
    X = 0.0;
}

ScanData::ScanData()
{
  //points = NULL;
  //SetPointCount(101);
}

void ScanData::SetPointCount(int n)
{
  //point_count=n;
  //points = (Sample *)realloc(points,sizeof(Sample)*point_count);
  //for (int i=0;i<point_count;i++)
  //    points[i].Sample();
    points.resize(n+1);
  //printf("size=%d\n",points.size());
}

int ScanData::GetPointCount()
{
    return points.size()-1;
}

void ScanData::UpdateStats()
{
    swr_min_idx = Z_min_idx = X_min_idx = R_min_idx = Z_max_idx = X_max_idx = R_max_idx = swr_max_idx = 0;

    freq_start = points.front().freq;
    freq_end = points.back().freq;

    for (unsigned int i=0;i<points.size();i++)
    {
        //double
           //n = points[i].Z*points[i].Z - points[i].R*points[i].R;

        //points[i].X2 = copysign(sqrt(abs(n)),n);
//printf("i=%d, swr=%lf, f=%lf\n",i,points[i].swr,points[i].freq);
        if (points[i].swr < points[swr_min_idx].swr) { swr_min_idx=i; }
        if (points[i].swr > points[swr_max_idx].swr) { swr_max_idx=i; }
        if (points[i].Z < points[Z_min_idx].Z) { Z_min_idx=i; }
        if (points[i].Z > points[Z_max_idx].Z) { Z_max_idx=i; }
        if (points[i].X < points[X_min_idx].X) { X_min_idx=i; }
        if (points[i].X > points[X_max_idx].X) { X_max_idx=i; }
        if (points[i].R < points[R_min_idx].R) { R_min_idx=i; }
        if (points[i].R > points[R_max_idx].R) { R_max_idx=i; }
        //if (points[i].X2 < points[X2_min_idx].X2) { X2_min_idx=i; }
        //if (points[i].X2 > points[X2_max_idx].X2) { X2_max_idx=i; }
    }
    for (int i=swr_bw_lo_idx=swr_min_idx;i>=0 && i<(int)points.size() && points[i].swr<=Config::swr_bw_max;i--)
        swr_bw_lo_idx=i;
    for (int i=swr_bw_hi_idx=swr_min_idx;i<(int)points.size() && points[i].swr<=Config::swr_bw_max;i++)
        swr_bw_hi_idx=i;


//fflush(stdout);
}

void ScanData::dummy_data(EventReceiver *erx)
{
    //freq_start = 26205000.0;
    //freq_end = 28205000.0;
    double freq_inc = (freq_end-freq_start)/GetPointCount();

    for (unsigned int i=0;i<points.size();i++)
    {
        points[i].freq = freq_start + i*freq_inc;
        points[i].swr = 5.1 + sin(double(i)/(points.size()-1)*2*3.14159)*4.0;
        points[i].Z = 100 + cos(double(i)/(points.size()-1)*2*3.14159)*50;
//printf("%d: %lf => %lf\n",i,points[i].freq,points[i].swr);
        erx->RaiseEvent(EventReceiver::progress_event, i*100/(points.size()-1));
    }
//fflush(stdout);
    UpdateStats();
}

void ScanData::toDom(QDomDocument &doc,QDomElement &parent)
{
    QDomElement element = doc.createElement("scandata");
    unsigned int i;
    QDomElement point;

    element.setAttribute(QString("fstart"),freq_start);
    element.setAttribute(QString("fend"),freq_end);

    //version.toDom(doc,ca);

    //toDom_Text(doc,element,"fstart",freq_start);
    //toDom_Text(doc,element,"fend",freq_end);

    for (i=0;i<points.size();i++)
    {
        point = doc.createElement("point");
        point.setAttribute(QString("freq"), points[i].freq);
        toDom_Text(doc,point,"SWR",points[i].swr);
        toDom_Text(doc,point,"Z",points[i].Z);
        toDom_Text(doc,point,"X",points[i].X);
        toDom_Text(doc,point,"R",points[i].R);
        element.appendChild(point);
    }

    parent.appendChild(element);
}

bool ScanData::fromDom(QDomElement &e0)
{
    Sample point;
//printf("ScanData::fromDom: e0=%s\n",e0.tagName().toAscii().data());

    freq_start = e0.attribute("fstart", "0").toDouble();
    freq_end = e0.attribute("fend", "0").toDouble();

    points.clear();

    for (QDomNode n1 = e0.firstChild(); !n1.isNull(); n1 = n1.nextSibling())
    {
        if (!n1.isElement()) continue;  // Skip any non-element nodes

        QDomElement e1 = n1.toElement();
        if (e1.tagName() == "point")
        {
            point.freq = e1.attribute("freq", "0").toDouble();

            for (QDomNode n2 = e1.firstChild(); !n2.isNull(); n2 = n2.nextSibling())
            {
                if (!n2.isElement()) continue;  // Skip any non-element nodes

                QDomElement e2 = n2.toElement();
                if (e2.tagName() == "SWR")      { point.swr = e2.text().toDouble(); }
                else if (e2.tagName() == "Z")	{ point.Z = e2.text().toDouble(); }
                else if (e2.tagName() == "X")	{ point.X = e2.text().toDouble(); }
                else if (e2.tagName() == "R")	{ point.R = e2.text().toDouble(); }
            }

            points.push_back(point);
        }
    }

    UpdateStats();

    return true;
}

void Sample::fromRaw(double vf,double vr,double vz,double va)
{
    swr = (vf + vr) / (vf - vr);
    Z = 50.0 * vz/va;
    R = ((2500.0 + Z*Z) * swr)/(50.0 * (swr*swr + 1));
    X = Z>R ? sqrt(Z*Z - R*R) : -sqrt(-Z*Z + R*R);
}
