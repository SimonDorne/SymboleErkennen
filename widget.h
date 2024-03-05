#ifndef WIDGET_H
#define WIDGET_H

#include<QWidget>
#include<QGraphicsView>
#include<QGraphicsEllipseItem>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsLineItem>
#include<QMouseEvent>
#include<iostream>
#include<cmath>
#include<QGraphicsView>
#include<QGraphicsPolygonItem>
#include<QBrush>
#include<QPen>
#include<QPointF>
#include<QTimer>
#include<QCursor>
#include<QGraphicsLineItem>
#include<chrono>

class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void updatePosition();
    void setCursorcircle(QPointF point);
    void mausClick(QPointF dummy);

    ~Widget();

private:    //m_ for memberVariablen von widget.h Widget:: Prefix wird in widget.cpp weggelassen

    QGraphicsEllipseItem * m_circlecursor=nullptr;
};
#endif // WIDGET_H
// WIDGET_H
