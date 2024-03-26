#include"widget.h"
#include"customscene.h"
#include"NeuralNetwork.h"
#include <iostream>
#include<fstream>
#include <QThread>
#include <sstream>


Widget::Widget(QWidget *parent): QGraphicsView(new CustomScene(), parent),nn(std::vector<uint32_t>{676, 9, 3}, 0.2){
    QTimer * timer = new QTimer (this);
    generator.seed(std::time(nullptr));
    scene() -> setSceneRect (0 ,0 ,600 ,700);
    //Größe des Fensters in welchem alle Buttons, Fenster und co stattfinden
    scene()->setBackgroundBrush(Qt::lightGray);


    timer ->start (20); // ergibt einen Frame pro Millisekunde
    //bei jedem Durchlauf der Main wird nach einem Delay von einer Millisekunde die Funktion updatePosition aufgerufen ->Animation des Balls
    connect (timer , & QTimer :: timeout , this , & Widget :: updatePosition );



    m_kreis= new QPushButton("Kreis", this);
    m_kreis->setGeometry(30,20,100,20);
    m_viereck=new QPushButton("Viereck", this);
    m_viereck->setGeometry(170,20,100,20);
    m_dreieck=new QPushButton("Dreieck", this);
    m_dreieck->setGeometry(310,20,100,20);
    m_radierer=new QPushButton("Radierer", this);
    m_radierer->setGeometry(450,20,100,20);
    m_train=new QPushButton("Trainieren", this);
    m_train->setGeometry(30,590,520,20);
    m_guess=new QPushButton("Raten", this);
    m_guess->setGeometry(30,630,520,20);
    m_generate=new QPushButton("Generiere Trainingsdaten", this);
    m_generate->setGeometry(30,670,520,20);
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

    connect(m_radierer, &QPushButton::clicked, this, &Widget::setMalbarTrue);

    connect(m_train, &QPushButton::clicked, this, &Widget::train);

    connect(m_guess, &QPushButton::clicked, this, &Widget::symbolRaten);

    connect(m_generate, &QPushButton::clicked, this, &Widget::generateTrainingdata);


}

void Widget::generateTrainingdata(){

        std::ofstream outputFile("/home/simon/Dokumente/C++Kurs/ProjektSymboleErkennen/Daten/targetInput.txt",std::ios::app);
        // Überprüfen, ob die Datei erfolgreich geöffnet wurde
        if (!outputFile.is_open()) {
            std::cerr << "Fehler beim Öffnen der Datei." << std::endl;
        }


    for(int i=0;i<9000;i++){
        if(i<3000){
            paintrect(); //output 1,0,0
            std::cout<<"Rectangle "<<i<<" simulated"<<std::endl;
        }
        else if(i>=3000&&i<6000){
            paintcirc(); //output 0,1,0
            std::cout<<"Circle "<<i<<"  simulated"<<std::endl;
        }
        else{
            painttriang();  //output 0,0,1
            std::cout<<"Triangle "<<i<<"  simulated"<<std::endl;
        }
        checkPapier();
        // Array-Inhalte in die Datei schreiben
        for (int i = 0; i < m_pixelValue.size(); ++i) {
            outputFile << m_pixelValue[i]<<" ";
        }
        // Datei schließen
        outputFile <<std::endl;
        setMalbarTrue();
    }
    outputFile.close();

    outputFile.open("/home/simon/Dokumente/C++Kurs/ProjektSymboleErkennen/Daten/targetOutput.txt",std::ios::app);

    for(int i=0;i<9000;i++){
        if(i<3000){
            outputFile <<"1.0 0.0 0.0"<<std::endl;
        }
        else if(i>3000&&i<6000){
            outputFile <<"0.0 1.0 0.0"<<std::endl;
        }
        else{
            outputFile <<"0.0 0.0 1.0"<<std::endl;
        }
    }
    outputFile.close();
    std::cout<<"Training Data generated"<<std::endl;
}

void Widget::train(){
    std::cout<<"Training starts"<<std::endl;
    //Machine learning
    std::vector<std::vector<float>> targetInputs;
    std::vector<std::vector<float>> targetOutputs;
    std::ifstream inputFile("/home/simon/Dokumente/C++Kurs/ProjektSymboleErkennen/Daten/targetInput.txt");
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        float value;
        std::vector<float> row;
        // Jeden Eintrag in der Zeile lesen und zum aktuellen Vektor hinzufügen
        while (iss >> value) {
            row.push_back(value);
        }
        // Den aktuellen Vektor zu unserem Vektor von Vektoren hinzufügen
        targetInputs.push_back(row);
    }
    // Datei schließen
    inputFile.close();

    inputFile.open("/home/simon/Dokumente/C++Kurs/ProjektSymboleErkennen/Daten/targetOutput.txt");
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        float value;
        std::vector<float> row;
        // Jeden Eintrag in der Zeile lesen und zum aktuellen Vektor hinzufügen
        while (iss >> value) {
            row.push_back(value);
        }
        // Den aktuellen Vektor zu unserem Vektor von Vektoren hinzufügen
        targetOutputs.push_back(row);
    }
    // Datei schließen
    inputFile.close();
    std::uniform_int_distribution<> dist(0, (targetInputs.size())-1);
    uint32_t epoch=3000;
    std::cout << "Training started" << std::endl;
    uint32_t index=0;
    int epochtriang=0;
    int epochrect=0;
    int epochcirc=0;
    for(int i=0;epochtriang<4 || epochrect<4 || epochcirc<4;i++){
        epochtriang=0;
        epochrect=0;
        epochcirc=0;
        for(uint32_t j=0;j<epoch;j++){
            index=dist(generator);
            //std::cout<<index<<std::endl;
            nn.FeedForward(targetInputs[index]);
            //std::cout << "Feedforward Round "<<i<< std::endl;
            nn.backPropagate(targetOutputs[index]);
            //std::cout << "Propagate Backwards Round "<<i<< std::endl;
            std::cout<<"Zyklus"<<i*epoch+j<<std::endl;
        }
        std::cout<<"Circle Check"<<std::endl;
        for(int j=0;j<5;j++){
            paintcirc();
            checkPapier();
            nn.FeedForward(m_pixelValue);
            std::vector<float> preds = nn.getPrediction();
            std::cout <<"Rechteck:"<< preds[0]<<"   Kreis:"<<preds[1]<<"   Dreieck:"<<preds[2] << std::endl;
            if(preds[0]+preds[2]<0.3&&preds[1]>0.7){
                epochcirc++;
            }
            setMalbarTrue();
        }
        std::cout<<"Rectangle Check"<<std::endl;
        for(int j=0;j<5;j++){
            paintrect();
            checkPapier();
            nn.FeedForward(m_pixelValue);
            std::vector<float> preds = nn.getPrediction();
            std::cout <<"Rechteck:"<< preds[0]<<"   Kreis:"<<preds[1]<<"   Dreieck:"<<preds[2] << std::endl;
            if(preds[1]+preds[2]<0.3&&preds[0]>0.7){
                epochrect++;
            }
            setMalbarTrue();
        }
        std::cout<<"Triangle Check"<<std::endl;
        for(int j=0;j<5;j++){
            painttriang();
            checkPapier();
            nn.FeedForward(m_pixelValue);
            std::vector<float> preds = nn.getPrediction();
            std::cout <<"Rechteck:"<< preds[0]<<"   Kreis:"<<preds[1]<<"   Dreieck:"<<preds[2] << std::endl;
            if(preds[0]+preds[1]<0.3&&preds[2]>0.7){
                epochtriang++;
            }
            setMalbarTrue();
        }
    }
    std::cout << "Training completed" << std::endl;
    nn.saveToFile("/home/simon/Dokumente/C++Kurs/ProjektSymboleErkennen/Daten/neuralnetwork.txt");
}


void Widget::symbolRaten(){
    if(m_malbar==false){
        checkPapier();
        nn.loadFromFile("/home/simon/Dokumente/C++Kurs/ProjektSymboleErkennen/Daten/neuralnetwork.txt");
        nn.FeedForward(m_pixelValue);
        std::vector<float> preds = nn.getPrediction();
        std::cout <<"Rechteck:"<< preds[0]<<"   Kreis:"<<preds[1]<<"   Dreieck:"<<preds[2] << std::endl;
        setMalbarTrue();
    }
}

void Widget::checkPapier(){
    for(int x=0;x<520;x=x+20){
        for(int y=0;y<520;y=y+20){
            m_pixel.setRect(x,y,20,20);
            if(m_pixel.collidesWithItem(m_line)){
                float sum=0;
                for(int x1=0;x1<20;x1=x1+2){
                    for(int y1=0;y1<20;y1=y1+2){
                        m_pixel.setRect(x+x1,y+y1,2,2);
                        if(m_pixel.collidesWithItem(m_line)){
                            sum=sum+1.0;
                        }
                    }
                }
                m_pixelValue.push_back(sum/(20*20));
                //std::cout<<sum/(400)<<std::endl;
            }
            else{
                m_pixelValue.push_back(0);
            }
        }
    }
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
            scene()->addItem(m_line);// Linie zur Szene hinzufügen}
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

    if(std::abs(x1 - x2) > 30 && std::abs(x1 - x3) > 30 && std::abs(x2 - x3) > 30 &&
        std::abs(y1 - y2) > 30 && std::abs(y1 - y3) > 30 && std::abs(y2 - y3) > 30){

        std::vector<QPointF> dreieck = {QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3), QPointF(x1, y1)};


        QPainterPath pfad;
        pfad.moveTo(dreieck[0]);         //Path beginnt bei erstem Element von m_points array
        for (size_t i = 1; i < dreieck.size(); ++i) {      //for schleife über array
            pfad.lineTo(dreieck[i]);                     //mache linien zwischen den Punkten welche
        }
        m_line = new QGraphicsPathItem(pfad);

        if(m_line->collidesWithItem(m_papier, Qt::ContainsItemShape)){
            m_line->setPen(QPen(Qt::red, 10)); // Stiftfarbe und -dicke einstellen
            scene()->addItem(m_line);
            m_malbar=false;
        }
        else {
            painttriang();
        }
    }
    else{
        painttriang();
    }

    //testen ob gemalt wurde
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

    // Standardabweichung für die Normalverteilung
     // Standardabweichung

     // Normalverteilte Zufallszahlen für x- und y-Koordinaten
     std::normal_distribution<double> dist_x(m_mean_x, m_stddev);
     std::normal_distribution<double> dist_y(m_mean_y, m_stddev);


        //std::cout<<abweichung1<<" "<<abweichung2<<" "<<abweichung3 <<" "<<abweichung4<<std::endl;

        // Zufällige x- und y-Koordinaten generieren
        const int x1 = std::round(dist_x(gen));
        const int y1 = std::round(dist_y(gen));

        const int x2 = std::round(dist_x(gen));
        const int y2 = std::round(dist_y(gen));

        if(std::abs(x1-x2)>30&&std::abs(y1-y2)>30){
            std::uniform_int_distribution<> dist(0,10);
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
                QPointF(x1, y1)
            };

            QPainterPath pfad;
            pfad.moveTo(rechteck[0]);         //Path beginnt bei erstem Element von m_points array
            for (size_t i = 1; i < rechteck.size(); ++i) {      //for schleife über array
                pfad.lineTo(rechteck[i]);                     //mache linien zwischen den Punkten welche
            }
            m_line = new QGraphicsPathItem(pfad);
            if(m_line->collidesWithItem(m_papier, Qt::ContainsItemShape)){
                m_line->setPen(QPen(Qt::red, 10)); // Stiftfarbe und -dicke einstellen
                scene()->addItem(m_line);
                m_malbar=false;
            }
            else {
                paintrect();
            }
        }
        else{
            paintrect();
        }
        //testen ob gemalt wurde
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


    //kreismitte ist auf papier
    if (m_papier->contains(kreismitte)){
        QRectF rect = m_papier->rect();

        qreal minDistance = std::min({
            std::abs(rect.left() - x),
            std::abs(rect.right() - x),
            std::abs(rect.top() - y),
            std::abs(rect.bottom() - y)
        });
        //35, da halbachsen mindestens 31 plus 3 abweichung
        if(minDistance-35>0){
            std::uniform_int_distribution<> dist2(31, minDistance - 3);
            std::uniform_int_distribution<> dist3(-3, 3);

            const int halbachse_a = dist2(generator);
            const int halbachse_b =halbachse_a +5*dist3(generator);
            std::uniform_int_distribution<> dist_angle(0, 360); // Winkel der Hauptachse (0-360 Grad)
            const int winkel = dist_angle(generator);

            int grad = 0;
            for (int i = 0; i < kreispunkte; ++i) {
                int deviation_x = dist3(generator);
                int deviation_y = dist3(generator);

                double theta = 2 * M_PI * i / kreispunkte;

                qreal x_coord = x + deviation_x + halbachse_a * std::cos(theta) * std::cos(winkel * M_PI / 180.0) - halbachse_b * std::sin(theta) * std::sin(winkel * M_PI / 180.0);
                qreal y_coord = y + deviation_y + halbachse_a * std::cos(theta) * std::sin(winkel * M_PI / 180.0) + halbachse_b * std::sin(theta) * std::cos(winkel * M_PI / 180.0);


                //qreal x_coord = x + deviation_x + radius * std::cos(grad * M_PI / 180.0);
                //qreal y_coord = y + deviation_y + radius * std::sin(grad * M_PI / 180.0);

                kreis[i] = QPointF(x_coord, y_coord);
                grad = (grad + 360 / kreispunkte) % 360; // Ensure grad stays within 0-359
            }

            kreis.push_back(kreis[0]);
            QPainterPath pfad;
            pfad.moveTo(kreis[0]);
            for (size_t i = 1; i < kreis.size(); ++i) {
                pfad.lineTo(kreis[i]);
            }
            //da kreismitte auf papier und halbachsen entsprechend gewählt sind, sollte dies immer gehen und muss nicht
            //extra gecheckt werden
            m_line = new QGraphicsPathItem(pfad);
            m_line->setPen(QPen(Qt::red, 10)); // Stiftfarbe und -dicke einstellen
            scene()->addItem(m_line);
            m_malbar=false;
        }
        else{
            paintcirc();
        }
    }
    else{
        paintcirc();
    }
}

//Diese Funktion setzt die Bool Variable m_malbar auf true und resetted davor einige Inhalte
void Widget::setMalbarTrue() {
        m_points.clear();
        m_path.clear();
        clearDrawing(); //löscht das bisher Gemalte
        m_malbar = true;
        m_pixelValue.clear();
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
    clearDrawing();
    m_path.clear();
    m_path.moveTo(m_points[0]);         //Path beginnt bei erstem Element von m_points array
    for (size_t i = 1; i < m_points.size(); ++i) {      //for schleife über array
        m_path.lineTo(m_points[i]);                     //mache linien zwischen den Punkten welche
    }
    m_line = new QGraphicsPathItem(m_path);
    m_line->setPen(QPen(Qt::black, 10)); // Stiftfarbe und -dicke einstellen
    scene()->addItem(m_line);
    m_path.clear();
    m_malbar=false;
}

//Destruktor
Widget::~Widget(){

}
