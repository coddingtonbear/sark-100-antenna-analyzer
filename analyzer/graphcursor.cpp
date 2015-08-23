#include "graphcursor.h"

GraphCursor::GraphCursor(QWidget *parent) :
    QWidget(parent)
{
}

void GraphCursor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(Qt::magenta);
    painter.drawLine(0,0,0,height());
    painter.end();
}
