#include <stdio.h>

#include <QPainter>

#include "config.h"

#include "scandata.h"

#include "graphcanvas.h"

extern ScanData scandata;

GraphCanvas::GraphCanvas(QWidget *parent) :
    QFrame(parent)
{
    //TODO: most of this function should be defined elsewhere leaving this class generic

    QPen pen1(Qt::black), pen2(Qt::blue), pen3(QColor(255,85,0)), pen4(Qt::red), pen5(Qt::green);

    pen1.setWidth(0);
    pen2.setWidth(0);
    pen3.setWidth(0);
    pen4.setWidth(0);
    pen5.setWidth(0);

    QFont font("Arial",9);

    xscale = new GraphScale(&graph, GraphScale::pos_bottom);
    xscale->pen = pen1;  //TODO: tidy
    xscale->font = font;
    xscale->labdps = 1;
    xscale->labdiv = 1000000;
    graph.AddItem(xscale);

    yscale1 = new GraphScale(&graph, GraphScale::pos_left);
    yscale1->pen = pen1;  //TODO: tidy
    yscale1->font = font;
    yscale1->title = "VSWR";
    //yscale1->vmin = 1.0;
    //yscale1->vmax = 10.0;    //TODO: from data
    //yscale1->vinc = 1.0;
    graph.AddItem(yscale1);

    yscale2 = new GraphScale(&graph, GraphScale::pos_right);
    yscale2->pen = pen1;  //TODO: tidy
    yscale2->font = font;
    yscale2->title = QString("Z,R,X %1").arg(QChar(0x03A9));
    //yscale2->vmin = 0.0;
    //yscale2->vmax = 200.0;    //TODO: from data
    //yscale2->vinc = 10.0;
    graph.AddItem(yscale2);

    swrtrace = new GraphTrace(&graph,yscale1);
    swrtrace->pen = pen2;  //TODO: tidy
    graph.AddItem(swrtrace);

    swrminline = new GraphVertLine(&graph,xscale);
    swrminline->pen = QPen(Qt::blue,0,Qt::DashLine);
    graph.AddItem(swrminline);

    ztrace = new GraphTrace(&graph,yscale2);
    ztrace->pen = pen3;  //TODO: tidy
    graph.AddItem(ztrace);

    xtrace = new GraphTrace(&graph,yscale2);
    xtrace->pen = QPen(Qt::red,0,Qt::DashLine);
    graph.AddItem(xtrace);

    //x2trace = new GraphTrace(&graph,yscale2);
    //x2trace->pen = QPen(Qt::red,0,Qt::DashLine);  //TODO: tidy
    //graph.AddItem(x2trace);

    rtrace = new GraphTrace(&graph,yscale2);
    rtrace->pen = pen5;  //TODO: tidy
    graph.AddItem(rtrace);

    ZTargetline = new GraphHorizLine(&graph,yscale2);
    ZTargetline->pen = QPen(Qt::black,0,Qt::DashLine);
    graph.AddItem(ZTargetline);

    SWRTargetline = new GraphHorizLine(&graph,yscale1);
    SWRTargetline->pen = QPen(Qt::blue,0,Qt::DashLine);
    graph.AddItem(SWRTargetline);
}

GraphCanvas::~GraphCanvas()
{
    delete xscale;
    delete yscale1;
    delete yscale2;
    delete swrtrace;
    delete ztrace;
}

void GraphCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

//cursor->move(0,0);
//cursor->resize(1,height());

    ////////////////////

    painter.setBackgroundMode(Qt::OpaqueMode); //??
    painter.setBackground(Qt::white);
    painter.eraseRect(painter.viewport());

    painter.setBackgroundMode(Qt::TransparentMode); //??

    graph.SetSize(painter.viewport());

    graph.Draw(painter);

    ////////////////////

    painter.end();
}

void GraphCanvas::mouseMoveEvent(QMouseEvent *ev)
{
    int x = ev->x();
    if (x<graph.marginl) x=graph.marginl;
    if (x>(width()-graph.marginr)) x=width()-graph.marginr;

    cursor->move(x,graph.margint);
    cursor->resize(1,height()-graph.margint-graph.marginb);

    emit cursorMoved((double)(x-graph.marginl)/(double)(width()-graph.marginl-graph.marginr));
}
