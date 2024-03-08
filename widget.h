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
#include <QPushButton>
#include <QVBoxLayout>
#include <QButtonGroup>
#include<QGraphicsRectItem>
#include<QPainterPath>

class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void updatePosition();
    void setCursorcircle(QPointF point);
    void mausClick(QPointF dummy);
    void clearDrawing();
    ~Widget();

private:    //m_ for memberVariablen von widget.h Widget:: Prefix wird in widget.cpp weggelassen

    QGraphicsEllipseItem * m_circlecursor=nullptr;

    bool m_malbar=true;
    std::vector<QPointF> m_points;
    QGraphicsRectItem *m_papier=nullptr;
    QPushButton *m_kreis = nullptr;
    QPushButton *m_dreieck = nullptr;
    QPushButton *m_viereck = nullptr;
    QPushButton *m_undefinierbar=nullptr;
    QPainterPath m_path;
    QGraphicsPathItem *m_line;


private slots:
    void setMalbarTrue();



};
#endif // WIDGET_H
// WIDGET_H
