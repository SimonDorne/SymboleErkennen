#include "customscene.h"
#include<iostream>
#include <chrono>
CustomScene::CustomScene(QObject *parent) :
    QGraphicsScene()
{
    Q_UNUSED(parent);
}

CustomScene::~CustomScene()
{

}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){

    auto b = std::chrono::high_resolution_clock::now();
    double mytime = std::chrono::duration<double>(b - m_begin_time).count();
    emit clickTargetCoordinate(event->scenePos(), mytime);
    m_mouse_is_pressed = false;
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event){

    m_begin_time = std::chrono::high_resolution_clock::now();
    m_mouse_is_pressed = true;

}



