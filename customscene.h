#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomScene(QObject *parent = 0);
    ~CustomScene();

    auto begin_time() const { return m_begin_time; }
    auto mouse_is_pressed() const { return m_mouse_is_pressed; }

signals:
    void signalTargetCoordinate(QPointF point);
    //-> Koordinaten werden bei mouseMoveEvent übergeben
    void clickTargetCoordinate(QPointF point, double mytime);
    //-> Variablen werden bei mouseReleaseEvent übergeben

public slots:

private:
    // The function, which made tracking the mouse
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool m_mouse_is_pressed = false;
    decltype(std::chrono::high_resolution_clock::now()) m_begin_time = std::chrono::high_resolution_clock::now();
};

#endif // CUSTOMSCENE_H
