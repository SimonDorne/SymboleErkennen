#include"widget.h"
#include"customscene.h"

static const QPointF points[3] = {
    QPointF(10.0, 80.0),
    QPointF(20.0, 10.0),
    QPointF(80.0, 30.0),
};


Widget::Widget(QWidget *parent): QGraphicsView(new CustomScene(), parent){
    QTimer * timer = new QTimer (this);
    scene() -> setSceneRect (0 ,0 ,600 ,700);
    //Größe des Fensters in welchem alle Buttons, Fenster und co stattfinden
    scene()->setBackgroundBrush(Qt::lightGray);



    timer ->start (10); // ergibt einen Frame pro Millisekunde
    //bei jedem Durchlauf der Main wird nach einem Delay von einer Millisekunde die Funktion updatePosition aufgerufen ->Animation des Balls
    connect (timer , & QTimer :: timeout , this , & Widget :: updatePosition );


    m_kreis= new QPushButton("Kreis", this);
    m_kreis->setGeometry(25,20,100,20);
    m_viereck=new QPushButton("Viereck", this);
    m_viereck->setGeometry(170,20,100,20);
    m_dreieck=new QPushButton("Dreieck", this);
    m_dreieck->setGeometry(320,20,100,20);
    m_undefinierbar=new QPushButton("Undefiniert", this);
    m_undefinierbar->setGeometry(470,20,100,20);


    //papier ist das Areal, in welchem gezeichnet werden kann
    m_papier=new QGraphicsRectItem();
    m_papier->setRect(25,50,555,600);
    m_papier->setBrush(QBrush(Qt::white));
    scene()->addItem(m_papier);


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
    //durch bool Variablen in customscene kann überprüft werden ob die maus gedrückt ist und bereits davor released wurde
    //nur zwischem dem ersten drücken und dem ersten release kann gemalt werden

    //connected sind momentan alle nur mit der gleichen Funktion, welche resetted
    connect(m_kreis, &QPushButton::clicked, this, &Widget::setMalbarTrue);
    connect(m_viereck, &QPushButton::clicked, this, &Widget::setMalbarTrue);
    connect(m_dreieck, &QPushButton::clicked, this, &Widget::setMalbarTrue);
    connect(m_undefinierbar, &QPushButton::clicked, this, &Widget::setMalbarTrue);



}

 //updatePosition ist für alles zuständig was irgendwie animiert sein soll
//ist mit timer connected um Animationen mit Delay durchzuführen (Aktualisierung der Frames zb 1 ms)
void Widget::updatePosition(){
    auto maus=static_cast<CustomScene *>(scene())->mouse_is_pressed(); //speicher ob maus gedrückt ist
    if(m_papier->collidesWithItem(m_circlecursor,Qt::IntersectsItemShape)){ //wenn maus auf papier ist:
       // std::cout<<"1"<<std::endl;
       if(m_malbar==true&&maus==true){ //maus gedrückt und noch nichts davor gemalt
            //      std::cout<<"test"<<std::endl;
            QPointF dot;                //mache einen dot
            dot.setX(m_circlecursor->pos().x());
            dot.setY(m_circlecursor->pos().y());
            m_points.push_back(dot);            //hänge ihn an ein QPointF (m_points)array an
            // QPainterPath m_path;
            m_path.moveTo(m_points[0]);         //Path beginnt bei erstem Element von m_points array
            for (size_t i = 1; i < m_points.size(); ++i) {      //for schleife über array
                m_path.lineTo(m_points[i]);                     //mache linien zwischen den Punkten welche
            }
            m_line = new QGraphicsPathItem(m_path);
            m_line->setPen(QPen(Qt::black, 2)); // Stiftfarbe und -dicke einstellen
            scene()->addItem(m_line); // Linie zur Szene hinzufügen}
        }
    }
   // else{std::cout<<"0"<<std::endl;}
}

//Diese Funktion setzt die Bool Variable m_malbar auf true und resetted davor einige Inhalte
void Widget::setMalbarTrue() {
    if(m_malbar==false){
        m_points.clear();
        m_path.clear();
        clearDrawing(); //löscht das bisher Gemalte
    }
    m_malbar = true;


}


void Widget::setCursorcircle(QPointF point){
    m_circlecursor->setPos(point);
    //Cursor ist da wo maus zuletzt ist (mouse move event)
}

//Löscht alle QGraphicsPathItem (m_line), welche auf dem Canvas für die Live Darstellung genutzt wird
void Widget::clearDrawing() {
    QList<QGraphicsItem*> items = scene()->items(); // Alle Objekte in der Szene erhalten
    for (QGraphicsItem* item : items) {
        if (item->type() == QGraphicsPathItem::Type) {
            // Wenn das Objekt ein QGraphicsPathItem ist, entfernen Sie es aus der Szene
            scene()->removeItem(item);
            delete item; // Speicher freigeben, da es nicht mehr benötigt wird
        }
    }
}


//wird bei mouse release event aktiviert
void Widget::mausClick(QPointF dummy){
    m_malbar=false;
}

//Destruktor
Widget::~Widget(){

}
