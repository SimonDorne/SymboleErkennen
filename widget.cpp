#include"widget.h"
#include"customscene.h"

static const QPointF points[3] = {
    QPointF(10.0, 80.0),
    QPointF(20.0, 10.0),
    QPointF(80.0, 30.0),
};




Widget::Widget(QWidget *parent): QGraphicsView(new CustomScene(), parent){
    QTimer * timer = new QTimer (this);
    generator.seed(std::time(nullptr));
    scene() -> setSceneRect (0 ,0 ,600 ,700);
    //Größe des Fensters in welchem alle Buttons, Fenster und co stattfinden
    scene()->setBackgroundBrush(Qt::lightGray);



    timer ->start (10); // ergibt einen Frame pro Millisekunde
    //bei jedem Durchlauf der Main wird nach einem Delay von einer Millisekunde die Funktion updatePosition aufgerufen ->Animation des Balls
    connect (timer , & QTimer :: timeout , this , & Widget :: updatePosition );



    m_kreis= new QPushButton("Kreis", this);
    m_kreis->setGeometry(30,20,100,20);
    m_viereck=new QPushButton("Viereck", this);
    m_viereck->setGeometry(170,20,100,20);
    m_dreieck=new QPushButton("Dreieck", this);
    m_dreieck->setGeometry(310,20,100,20);
    m_undefinierbar=new QPushButton("Undefiniert", this);
    m_undefinierbar->setGeometry(450,20,100,20);


    //papier ist das Areal, in welchem gezeichnet werden kann
    m_papier=new QGraphicsRectItem();
    m_papier->setRect(30,50,520,520);
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
    connect(m_kreis, &QPushButton::clicked, this, &Widget::paintcirc);
    //connect(m_kreis, &QPushButton::clicked, this, &Widget::setMalbarTrue);

    //connect(m_viereck, &QPushButton::clicked, this, &Widget::setMalbarTrue);
    connect(m_viereck, &QPushButton::clicked, this, &Widget::paintrect);
    //connect(m_dreieck, &QPushButton::clicked, this, &Widget::setMalbarTrue);
    connect(m_dreieck, &QPushButton::clicked, this, &Widget::painttriang);

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
            m_line->setPen(QPen(Qt::black, 10)); // Stiftfarbe und -dicke einstellen
            scene()->addItem(m_line); // Linie zur Szene hinzufügen}

            //Testen von paintbox();


        }
    }
   // else{std::cout<<"0"<<std::endl;}
}

void Widget::painttriang(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::normal_distribution<double> dist_x(m_mean_x, m_stddev);
    std::normal_distribution<double> dist_y(m_mean_y, m_stddev);

    const int x1 =  std::round(dist_x(generator));
    const int y1 = std::round(dist_y(generator));

    const int x2 =  std::round(dist_x(generator));
    const int y2 = std::round(dist_y(generator));

    const int x3 =  std::round(dist_x(generator));
    const int y3 = std::round(dist_y(generator));

    std::vector<QPointF> dreieck = {QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3), QPointF(x1, y1)};


    QPainterPath pfad;
    pfad.moveTo(dreieck[0]);         //Path beginnt bei erstem Element von m_points array
    for (size_t i = 1; i < dreieck.size(); ++i) {      //for schleife über array
        pfad.lineTo(dreieck[i]);                     //mache linien zwischen den Punkten welche
    }
    m_lineRed = new QGraphicsPathItem(pfad);

    if(m_lineRed->collidesWithItem(m_papier, Qt::ContainsItemShape)){
        m_lineRed->setPen(QPen(Qt::red, 10)); // Stiftfarbe und -dicke einstellen
        scene()->addItem(m_lineRed);
    }

    //testen ob gemalt wurde
    if(isItemInScene(m_lineRed, scene())==false){
        painttriang();
    }

}


void Widget::paintrect(){
   // m_papier->setRect(30,50,520,520);
   //links oben: 30,50
   //rechts oben: 530,50
   //links unten: 30, 550
   //rechts unten: 530, 550

    //Idee 20*20 Pixel, ->26x26 Raster aus diesen Pixeln
    //->676 Pixel als Aktivierungsneuronen


    std::random_device rd;
    std::mt19937 gen(rd());



    // Mittelpunkt des Rechtecks


    // Standardabweichung für die Normalverteilung
     // Standardabweichung

        // Normalverteilte Zufallszahlen für x- und y-Koordinaten
        std::normal_distribution<double> dist_x(m_mean_x, m_stddev);
        std::normal_distribution<double> dist_y(m_mean_y, m_stddev);


        //std::cout<<abweichung1<<" "<<abweichung2<<" "<<abweichung3 <<" "<<abweichung4<<std::endl;

        // Zufällige x- und y-Koordinaten generieren
        const int x1 =  std::round(dist_x(gen));
        const int y1 = std::round(dist_y(gen));

        const int x2 = std::round(dist_x(gen));
        const int y2 = std::round(dist_y(gen));

        std::uniform_int_distribution<> dist(0, 7);
        int abweichung1=dist(gen);
        int abweichung2=dist(gen);
        int abweichung3=dist(gen);
        int abweichung4=dist(gen);


        //Punkt1-Punkt3-Punkt2-Punkt4-Punkt1->
        //ist Pfad des Rechtecks mit Punkt 1 und Punkt 2 als zufallsgenerierte über die Diagonale verbundene Punkte
        std::vector<QPointF> rechteck={
            QPointF(x1, y1),
            QPointF(x1+abweichung1, y2+abweichung2),
            QPointF(x2, y2),
            QPointF(x2+abweichung3, y1+abweichung4),
            QPointF(x1, y1)};

        QPainterPath pfad;
        pfad.moveTo(rechteck[0]);         //Path beginnt bei erstem Element von m_points array
        for (size_t i = 1; i < rechteck.size(); ++i) {      //for schleife über array
            pfad.lineTo(rechteck[i]);                     //mache linien zwischen den Punkten welche
        }
        m_lineRed = new QGraphicsPathItem(pfad);

        if(m_lineRed->collidesWithItem(m_papier, Qt::ContainsItemShape)){
            m_lineRed->setPen(QPen(Qt::red, 10)); // Stiftfarbe und -dicke einstellen
            scene()->addItem(m_lineRed);
        }

        //testen ob gemalt wurde
        if(isItemInScene(m_lineRed, scene())==false){
            paintrect();
        }
}

void Widget::paintcirc() {
    std::uniform_int_distribution<> dist(25, 35);
    int kreispunkte = dist(generator);
    std::vector<QPointF> kreis(kreispunkte);

    std::normal_distribution<double> dist_x(m_mean_x, m_stddev);
    std::normal_distribution<double> dist_y(m_mean_y, m_stddev);
    const int x = std::round(dist_x(generator));
    const int y = std::round(dist_y(generator));
    QPointF kreismitte(x, y);

    if (m_papier->contains(kreismitte)) {
        QRectF rect = m_papier->rect();

        qreal minDistance = std::min({
            std::abs(rect.left() - x),
            std::abs(rect.right() - x),
            std::abs(rect.top() - y),
            std::abs(rect.bottom() - y)
        });

        std::uniform_int_distribution<> dist2(10, minDistance - 3);
        const int radius = dist2(generator);
        std::uniform_int_distribution<> dist3(-2, 2);
        int grad = 0;
        for (int i = 0; i < kreispunkte; ++i) {
            int deviation_x = dist3(generator);
            int deviation_y = dist3(generator);

            qreal x_coord = x + deviation_x + radius * std::cos(grad * M_PI / 180.0);
            qreal y_coord = y + deviation_y + radius * std::sin(grad * M_PI / 180.0);

            kreis[i] = QPointF(x_coord, y_coord);
            grad = (grad + 360 / kreispunkte) % 360; // Ensure grad stays within 0-359
        }

        kreis.push_back(kreis[0]);
        QPainterPath pfad;
        pfad.moveTo(kreis[0]);
        for (size_t i = 1; i < kreis.size(); ++i) {
            pfad.lineTo(kreis[i]);
        }

        QGraphicsPathItem* m_lineRed = new QGraphicsPathItem(pfad);

        if (m_lineRed->collidesWithItem(m_papier, Qt::ContainsItemShape)) {
            m_lineRed->setPen(QPen(Qt::red, 10));
            scene()->addItem(m_lineRed);
        }
        else {
            delete m_lineRed; // Avoid memory leak
        }
    }
    else{
        paintcirc();
    }
}

bool Widget::isItemInScene(QGraphicsItem *item, QGraphicsScene *scene) {
        QList<QGraphicsItem *> itemsInScene = scene->items();
        return itemsInScene.contains(item);
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
