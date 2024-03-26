#ifndef WIDGET_H
#define WIDGET_H

#include <ctime>
#include<random>
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
#include"NeuralNetwork.h"
class Widget : public QGraphicsView
{
    Q_OBJECT
    SimpleNeuralNetwork nn;

public:
    Widget(QWidget *parent = nullptr);
    void updatePosition();
    void setCursorcircle(QPointF point);
    void mausClick(QPointF dummy);
    void clearDrawing();
    void paintrect();
    void painttriang();
    void paintcirc();
    void checkPapier();
    void train();
    void symbolRaten();
    void generateTrainingdata();
    ~Widget();

private:    //m_ for memberVariablen von widget.h Widget:: Prefix wird in widget.cpp weggelassen

    QGraphicsEllipseItem * m_circlecursor=nullptr;

    bool m_malbar=true;
    bool m_rectIsPrinted=true;
    std::vector<QPointF> m_points;
    QGraphicsRectItem *m_papier=nullptr;
    QPushButton *m_kreis = nullptr;
    QPushButton *m_dreieck = nullptr;
    QPushButton *m_viereck = nullptr;
    QPushButton *m_radierer=nullptr;
    QPushButton *m_train=nullptr;
    QPushButton *m_guess=nullptr;
    QPushButton *m_generate=nullptr;
    QPainterPath m_path;
    QGraphicsPathItem *m_line;
    std::default_random_engine generator;
    QGraphicsRectItem m_pixel;
    std::vector<float> m_pixelValue;
    const int m_mean_x = 30 + 520 / 2;
    const int m_mean_y = 50 + 520 / 2;
    const double m_stddev = 200;
    std::vector<uint32_t> topology={676,9,3};



private slots:
    void setMalbarTrue();



};
#endif // WIDGET_H
// WIDGET_H
