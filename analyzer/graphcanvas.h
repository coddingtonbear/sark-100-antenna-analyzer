#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QFrame>
#include <QMouseEvent>

#include "graph.h"
#include "graphcursor.h"

class GraphCanvas : public QFrame
{
    Q_OBJECT
public:
    explicit GraphCanvas(QWidget *parent = 0);
    ~GraphCanvas();

    GraphCursor *cursor;

    Graph graph;
    GraphScale *xscale,*yscale1, *yscale2;
    GraphTrace *swrtrace, *ztrace, *xtrace, *rtrace;
    GraphVertLine *swrminline;
    GraphHorizLine *ZTargetline,*SWRTargetline;

signals:
    void cursorMoved(double pos);

public slots:

private:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *ev);
};

#endif // GRAPHCANVAS_H
