#include <math.h>
#include <stdio.h>

#include "graph.h"

Graph::Graph()
{
  marginl=marginb=marginr=margint=65;
}

void Graph::SetSize(QRect size)
{
    xo = size.left() + marginl;
    yo = size.bottom() - marginb;
    w = size.width() - marginl - marginr;
    h = size.height() - margint - marginb;
}

void Graph::AddItem(GraphItem *item)
{
    items.push_back(item);
}

void Graph::Draw(QPainter &painter)
{
    for (unsigned int i=0;i<items.size();i++)
        if (items[i]->enabled)
            items[i]->Draw(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

GraphItem::GraphItem(Graph *g)
{
    graph = g;
    enabled = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

GraphDataItem::GraphDataItem(Graph *g, GraphScale *s) : GraphItem(g)
{
    scale = s;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

GraphScale::GraphScale(Graph *g, pos_t p) : GraphItem(g)
{
    pos = p;
    vmin=0;
    vmax=10;
    vinc=1;
    pen = QPen(Qt::black);
    labdps = -1;
    labsuffix = "";
    title = "";
    labdiv = 1.0;
}

void GraphScale::Draw(QPainter &painter)
{
    double scale;
    int x,y,w,w_max,font_h = QFontMetrics(font).height();

    painter.setPen(pen);
    painter.setFont(font);

    switch (pos)
    {
      case pos_bottom:
        scale = graph->w / (vmax-vmin);
        painter.drawLine(graph->xo,graph->yo,graph->xo+graph->w,graph->yo);
        for (double v=vmin;v<=vmax;v+=vinc)
        {
            QString s = (labdps<0 ? QString("%1").arg(v/labdiv) : QString("%1").arg(v/labdiv, 0, 'f', labdps)) + labsuffix;

            x=graph->xo + (v-vmin)*scale;
        //printf("x=%d\n",x);
            painter.drawLine(x,graph->yo,x,graph->yo + 10);
            x=x - QFontMetrics(font).boundingRect(s).width()/2;
            painter.drawText(x,graph->yo + 10 + font_h,s);
        }
        break;

    case pos_left:
        scale = graph->h / (vmax-vmin);
        painter.drawLine(graph->xo,graph->yo,graph->xo,graph->yo-graph->h);
        w_max=0;
        for (double v=vmin;v<=vmax;v+=vinc)
        {
            QString s = (labdps<0 ? QString("%1").arg(v/labdiv) : QString("%1").arg(v/labdiv, 0, 'f', labdps)) + labsuffix;
            y=graph->yo - (v-vmin)*scale;
            painter.drawLine(graph->xo,y,graph->xo-10,y);
            w = QFontMetrics(font).boundingRect(s).width();
            x=graph->xo - 12 - w;
            painter.drawText(x,y + font_h/2.5,s);
            if (w>w_max) w_max=w;
        }
        if (!title.isEmpty())
        {
            x=graph->xo;
            y=graph->yo - graph->h/2.0;
            w=QFontMetrics(font).boundingRect(title).width();
            painter.save();
            painter.translate(x,y);
            painter.rotate(-90.0);
            painter.drawText(-w/2.0,-w_max-font_h-4,title);
            painter.restore();
        }
        break;

    case pos_right:
        scale = graph->h / (vmax-vmin);
        painter.drawLine(graph->xo+graph->w,graph->yo,graph->xo+graph->w,graph->yo-graph->h);
        w_max=0;
        for (double v=vmin;v<=vmax;v+=vinc)
        {
//printf("scalev=%lf\n",v);
            QString s = (labdps<0 ? QString("%1").arg(v/labdiv) : QString("%1").arg(v/labdiv, 0, 'f', labdps)) + labsuffix;
            x=graph->xo + graph->w;
            y=graph->yo - (v-vmin)*scale;
            w = QFontMetrics(font).boundingRect(s).width();
            painter.drawLine(x,y,x+10,y);
            painter.drawText(x+12,y + font_h/2.5,s);
            if (w>w_max) w_max=w;
        }
        if (!title.isEmpty())
        {
            x=graph->xo + graph->w;
            y=graph->yo - graph->h/2.0;
            w=QFontMetrics(font).boundingRect(title).width();
            painter.save();
            painter.translate(x,y);
            painter.rotate(90.0);
            painter.drawText(-w/2.0,-w_max-font_h-4,title);
            painter.restore();
        }
        break;

      default: break;
    }
}

void GraphScale::SetMinAuto()
{
    vmin = floor(vmin/vinc)*vinc;
}

void GraphScale::SetIncAuto()
{
    double n,span=this->vmax-this->vmin;
    n = pow(10.0,floor(log10(span)));
    if (span<=n) { this->vinc=n/10.0; return; } else n*=2.0;
    if (span<=n) { this->vinc=n/10.0; return; } else n*=2.5;
    if (span<=n) { this->vinc=n/10.0; return; } else n*=2;
    this->vinc=n/10.0;
}

void GraphScale::Expand(double min,double max)
{
    if (min<vmin) vmin=min;
    if (max>vmax) vmax=max;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//GraphHorizLine::GraphHorizLine(Graph *g, GraphScale *s) : GraphItem(g)
//{
//    scale = s;
//}

void GraphHorizLine::Draw(QPainter &painter)
{
    if (val>=scale->vmin && val<=scale->vmax)
    {
        int y = graph->yo - (val-scale->vmin)/(scale->vmax-scale->vmin)*graph->h;
        painter.setPen(pen);
        painter.drawLine(graph->xo,y,graph->xo+graph->w,y);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//GraphVertLine::GraphVertLine(Graph *g, GraphScale *s) : GraphItem(g)
//{
//    scale = s;
//}

void GraphVertLine::Draw(QPainter &painter)
{
    if (val>=scale->vmin && val<=scale->vmax)
    {
        int x = graph->xo + (val-scale->vmin)/(scale->vmax-scale->vmin)*graph->w;
        painter.setPen(pen);
        painter.drawLine(x,graph->yo,x,graph->yo-graph->h);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//GraphTrace::GraphTrace(Graph *g, GraphScale *s) : GraphItem(g)
//{
//    scale = s;
//}

void GraphTrace::Draw(QPainter &painter)
{
    painter.setPen(pen);
    for (unsigned int i=1;i<points.size();i++)
    {
        //printf("i=%d\n",i);
        painter.drawLine(graph->xo + (i-1)*graph->w/(points.size()-1),graph->yo - (points[i-1]-scale->vmin)/(scale->vmax-scale->vmin)*graph->h,
                graph->xo + i*graph->w/(points.size()-1),graph->yo - (points[i]-scale->vmin)/(scale->vmax-scale->vmin)*graph->h);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
