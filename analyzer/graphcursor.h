#ifndef GRAPHCURSOR_H
#define GRAPHCURSOR_H

#include <QWidget>
#include <QPainter>

class GraphCursor : public QWidget
{
    Q_OBJECT
public:
    explicit GraphCursor(QWidget *parent = 0);

signals:

public slots:

private:
    void paintEvent(QPaintEvent *);
};

#endif // GRAPHCURSOR_H
