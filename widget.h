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
    int m_anzahlhits=0;
    double m_velocity=0;
    double m_puls;
    double m_xdirection=1;
    double m_ydirection=1;
    QGraphicsTextItem * m_text=nullptr;
    QString m_ausgabeVersuche="";
    QGraphicsPolygonItem m_polygon;
    QPointF m_now, m_01, m_10, m_11 , mittelpunkt;
    QGraphicsEllipseItem * m_golfball=nullptr;
    QGraphicsEllipseItem * m_circlecursor=nullptr;
    QGraphicsEllipseItem * m_ziel=nullptr;
    QGraphicsLineItem * m_line=nullptr;
};
#endif // WIDGET_H
// WIDGET_H
