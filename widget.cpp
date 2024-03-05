#include"widget.h"
#include"customscene.h"



Widget::Widget(QWidget *parent): QGraphicsView(new CustomScene(), parent){
    QTimer * timer = new QTimer (this);
    scene() -> setSceneRect (0 ,0 ,1000 ,1000);
    //Größe des Fensters, im vgl Polygon ist von 10,10 bis 990,900
    scene()->setBackgroundBrush(Qt::lightGray);


    timer ->start (1); // ergibt einen Frame pro Millisekunde
    //bei jedem Durchlauf der Main wird nach einem Delay von einer Millisekunde die Funktion updatePosition aufgerufen ->Animation des Balls
    connect (timer , & QTimer :: timeout , this , & Widget :: updatePosition );


    //Cursor Hitbox mit Größe 10, schwarz
    //kann auf 1,1 gesetzt werden und ist nur für Debugging größer
    m_circlecursor = new QGraphicsEllipseItem (0 ,0 ,10 ,10);
    m_circlecursor->setPos(100,100);
    m_circlecursor->setBrush(QBrush(Qt::black));
    scene()->addItem(m_circlecursor);

    Widget::setMouseTracking(true);
    connect(static_cast<CustomScene *>(scene()), &CustomScene::signalTargetCoordinate, this, &Widget::setCursorcircle);
    //gibt bei Mausbewegung (mouseMoveEvent) Informationen an setCursorcircle weiter
    connect(static_cast<CustomScene *>(scene()), &CustomScene::clickTargetCoordinate, this, &Widget::mausClick);
    //gibt bei Maus loslassen (mouseReleaseEvent) Informationen an mausclick weiter

}

 //updatePosition ist für alles zuständig was irgendwie animiert sein soll
//ist mit timer connected um Animationen mit Delay durchzuführen (Aktualisierung der Frames zb 1 ms)
void Widget::updatePosition(){

}

void Widget::setCursorcircle(QPointF point){
    m_circlecursor->setPos(point);
    //Cursor ist da wo maus zuletzt ist (mouse move event)

}

//wird bei mouse release event aktiviert
void Widget::mausClick(QPointF dummy){


}




//Destruktor
Widget::~Widget(){

}
