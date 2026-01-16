#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDate>
#include <QMessageBox>
#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSet>
#include <QList>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // l'app s'ouvre toujours en premier sur le tab1
    ui->tabWidget->setCurrentIndex(0);

    //date
    QString dateDujour = QDate::currentDate().toString("dd/MM/yyyy");
    ui->lineEditDate->setPlaceholderText(dateDujour);

    //QCombobox depart & arivee
    connect(ui->boxVilleDepart, &QComboBox::currentTextChanged, this, [=](const QString &text){
        ui->boxVilleArivee->clear();
        if (text == "ANTANANARIVO")
            ui->boxVilleArivee->addItems({"FIANARANTSOA","TOAMASINA","MAHAJANGA"});
        else if (text == "FIANARANTSOA")
            ui->boxVilleArivee->addItems({"ANTANANARIVO","TOLIARA"});
        else if (text == "TOAMASINA")
            ui->boxVilleArivee->addItems({"ANTANANARIVO","DIEGO"});
        else if (text == "MAHAJANGA")
            ui->boxVilleArivee->addItems({"ANTANANARIVO","DIEGO"});
        else if (text == "TOLIARA")
            ui->boxVilleArivee->addItems({"FIANARANTSOA"});
        else if (text == "DIEGO")
            ui->boxVilleArivee->addItems({"MAHAJANGA","TOAMASINA", "ANTANANARIVO"});
    });

    //style boutton
    ui->pushButtonValider1->setStyleSheet(
            "QPushButton {"
            "background-color: rgb(0, 85, 0);"
            "}"
            "QPushButton::pressed {"
            "padding-top:10px;"
            "padding-left:10px;"
            "background-color: rgb(0, 85, 0);"
            "}");
    ui->pushButtonReserverClient->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(0, 85, 0);}"
        "QPushButton::pressed{"
        "background-color: rgb(38, 38, 38);}");
    ui->pushButtonValider2->setStyleSheet(
        "QPushButton {"
        "background-color: rgb(0, 170, 0);}"
        "QPushButton::hover {"
        "background-color: rgb(5, 12, 65);}"
        "QPushButton::pressed {"
        "background-color: rgb(38, 38, 38);}");


    //groupe categorie
    groupeCategories = new QButtonGroup(this);
    groupeCategories->setExclusive(true);

    groupeCategories->addButton(ui->pushButtonLite);
    groupeCategories->addButton(ui->pushButtonPremium);
    groupeCategories->addButton(ui->pushButtonVip);

    connect(groupeCategories, &QButtonGroup::buttonClicked, this, &MainWindow::onCategorieClicked);

    //groupe place
    place = new QButtonGroup(this);
    place->setExclusive(false);  // if false permet plusieurs section ( != true)

    place->addButton(ui->place1);
    place->addButton(ui->place2);
    place->addButton(ui->place3);
    place->addButton(ui->place4);
    place->addButton(ui->place5);
    place->addButton(ui->place6);
    place->addButton(ui->place7);
    place->addButton(ui->place8);
    place->addButton(ui->place9);
    place->addButton(ui->place10);
    place->addButton(ui->place11);
    place->addButton(ui->place12);
    place->addButton(ui->place13);
    place->addButton(ui->place14);
    place->addButton(ui->place15);
    place->addButton(ui->place16);
    place->addButton(ui->place17);
    place->addButton(ui->place18);
    place->addButton(ui->place19);
    place->addButton(ui->place20);

    connect(place, &QButtonGroup::buttonClicked, this, &MainWindow::onSetPlaceClicked);

    //icon status place
    iconFree = QIcon(":/icon/libre.png");
    iconSelected = QIcon(":/icon/reservee.png");
    qDebug() << "iconFree null:" << iconFree.isNull();
    qDebug() << "iconSelected null:" << iconSelected.isNull();

    //BASE DE DONNEES
    qDebug() << "Drivers SQl disponibles:" << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("clients.db");

    if(!db.open()) {
        qDebug() << "Erreur DB:" << db.lastError().text();
    }

    qDebug() << "DB utilisee:" << db.databaseName();
    qDebug() << "DB ouverte?:" << db.isOpen();

    QSqlQuery pragmaQ;
    pragmaQ.exec("PRAGMA foreign_keys = ON;");

    QSqlQuery q;

    //Table client
    if(!q.exec(R"(
           CREATE TABLE IF NOT EXISTS client(
           id INTEGER PRIMARY KEY AUTOINCREMENT,
           nom TEXT,
           prenom TEXT,
           contact TEXT,
           cin TEXT,
           email TEXT);
    )")) {
        qDebug() << "Failed to create table client:" << q.lastError().text();
    }

    //Table reservation
    if(!q.exec(R"(
           CREATE TABLE IF NOT EXISTS reservation(
           id INTEGER PRIMARY KEY AUTOINCREMENT,
           client_id INTEGER,
           villeDepart TEXT,
           villeArrivee TEXT,
           dateReservation TEXT,
           heureDepart TEXT,
           nbPlaceReservees INTEGER,
           nbBagages REAL,
           poidsBagages REAL,
           categorie TEXT,
           prix REAL,
           especes REAL,
           monnaie REAL,
           FOREIGN KEY(client_id) REFERENCES client(id));
    )")) {
        qDebug() << "Failed to create table reservation:" << q.lastError().text();
    }

    //Table reservation_place
    if(!q.exec(R"(
           CREATE TABLE IF NOT EXISTS reservation_place (
           id INTEGER PRIMARY KEY AUTOINCREMENT,
           reservation_id INTEGER,
           numPlace INTEGER,
           FOREIGN KEY(reservation_id) REFERENCES reservation(id));
    )")) {
        qDebug() << "Failed to create table reservation_place:" << q.lastError().text();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

Client client; // regroupe les info sur le client

void MainWindow::changerTab2()
{
    int indexActuel = ui->tabWidget->currentIndex();
    if (indexActuel + 1 == 2) {  // Si on va vers l'onglet des places
        mettreAJourplaces();
    }
    ui->tabWidget->setCurrentIndex(indexActuel + 1);
}


void MainWindow::on_pushButtonValider1_clicked()
{
    QString date= ui->lineEditDate->text();
    QString heure = ui->boxHeureDepart->currentText().trimmed(); //trimmed poue enlever les espaces
    QString depart = ui->boxVilleDepart->currentText().trimmed();
    QString arivee = ui->boxVilleArivee->currentText().trimmed();

    qDebug() << "Type choisi:" << heure; //pour que la variable soit utilisable sans warning
    qDebug() << "Type choisi:" << depart;
    qDebug() << "Type choisi:" << arivee;

    client.dateReservation = date;
    client.heureDepart = heure;
    client.villeDepart = depart;
    client.villeArivee = arivee;

    listesClients.append(client);

    //affichage des informations dans le billet
    ui->billetDate->setText(date);
    ui->billetVilleDepart->setText(depart);
    ui->billetvilleArivee->setText(arivee);
    ui->billetHeure->setText(heure);

    if(date.isEmpty()) {
       QMessageBox::warning(this,"erreur", "Remplisser tous les champs svp!");
    }

    changerTab2();

}



// Reservation speciale
void MainWindow::on_pushButtonReserverTanaManj_clicked()
{
    QString dateActuelle = QDate::currentDate().toString("dd/MM/yyyy");
    QString villeDepart = "ANTANANARIVO";
    QString villeArivee = "MAHANJANGA";
    QString heure = "05H00";

    qDebug() << "Date de reservation:" << dateActuelle;
    qDebug() << "Type choisi:" << villeDepart;
    client.dateReservation = dateActuelle;
    client.heureDepart = heure;
    qDebug() << "Type choisi:" << villeArivee;

    client.villeDepart = villeDepart;
    client.villeArivee = villeArivee;

    listesClients.append(client);

    //affichage des informations dans le billet
    ui->billetDate->setText(dateActuelle);
    ui->billetVilleDepart->setText(villeDepart);
    ui->billetvilleArivee->setText(villeArivee);
    ui->billetHeure->setText(heure);

    sauvegarderClient(client);

    changerTab2();
}



void MainWindow::on_pushButtonToliFian_clicked()
{
    QString dateActuelle = QDate::currentDate().toString("dd/MM/yyyy");
    QString villeDepart = "TOLIARA";
    QString villeArivee = "FIANARANTSOA";
    QString heure = "09H30";

    qDebug() << "Date de reservation:" << dateActuelle;
    qDebug() << "Type choisi:" << villeDepart;
    qDebug() << "Type choisi:" << villeArivee;

    client.dateReservation = dateActuelle;
    client.heureDepart = heure;
    client.villeDepart = villeDepart;
    client.villeArivee = villeArivee;

    listesClients.append(client);

    //affichage des informations dans le billet
    ui->billetDate->setText(dateActuelle);
    ui->billetVilleDepart->setText(villeDepart);
    ui->billetvilleArivee->setText(villeArivee);
    ui->billetHeure->setText(heure);

    sauvegarderClient(client);

    changerTab2();
}



void MainWindow::on_pushButtonTanaToam_clicked()
{
    QString dateActuelle = QDate::currentDate().toString("dd/MM/yyyy");
    QString villeDepart = "ANTANANARIVO";
    QString villeArivee = "TOAMASINA";
    QString heure = "14H00";


    qDebug() << "Date de reservation:" << dateActuelle;
    qDebug() << "Type choisi:" << villeDepart;
    qDebug() << "Type choisi:" << villeArivee;

    client.dateReservation = dateActuelle;
    client.heureDepart = heure;
    client.villeDepart = villeDepart;
    client.villeArivee = villeArivee;

    listesClients.append(client);

    //affichage des informations dans le billet
    ui->billetDate->setText(dateActuelle);
    ui->billetVilleDepart->setText(villeDepart);
    ui->billetvilleArivee->setText(villeArivee);
    ui->billetHeure->setText(heure);

    sauvegarderClient(client);

    changerTab2();
}



void MainWindow::on_pushButtonToamDiego_clicked()
{
    QString dateActuelle = QDate::currentDate().toString("dd/MM/yyyy");
    QString villeDepart = "TOAMASINA";
    QString villeArivee = "DIEGO";
    QString heure = "19H00";

    qDebug() << "Date de reservation:" << dateActuelle;
    qDebug() << "Type choisi:" << villeDepart;
    qDebug() << "Type choisi:" << villeArivee;

    client.dateReservation = dateActuelle;
    client.heureDepart = heure;
    client.villeDepart = villeDepart;
    client.villeArivee = villeArivee;

    listesClients.append(client);

    //affichage des informations dans le billet
    ui->billetDate->setText(dateActuelle);
    ui->billetVilleDepart->setText(villeDepart);
    ui->billetvilleArivee->setText(villeArivee);
    ui->billetHeure->setText(heure);

    sauvegarderClient(client);

    changerTab2();
}



void MainWindow::on_pushButtonReserverClient_clicked()
{
    QString nom = ui->lineEditNom->text();
    QString prenom = ui->lineEditPrenom->text();

    QString cin = ui->lineEditCIN->text();
    if(cin.isEmpty()) {
        QString afficher = "none";
        ui->billetCIN->setText(afficher);
    }

    QString email = ui->lineEditEmail->text();
    QString contact = ui->lineEditContact->text();

    int nbPersonne = ui->spinBoxNbPersonnes->value();
    int nbBagages = ui->spinBoxNbBagages->value();
    double poidsBagages = ui->doubleSpinBoxPoidsBagages->value();

    if(nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || contact.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veulliez rempilr les champs correctement🤔!");
    }

    qDebug() << "nom:" << nom;
    qDebug() << "prenom:" << prenom;
    qDebug() << "cin:" << cin;
    qDebug() << "email:" << email;
    qDebug() << "contact:" << contact;

    client.nom  = nom;
    client.prenom = prenom;
    client.cin = cin;
    client.email = email;
    client.contact = contact;
    client.nbPlaceReserver = nbPersonne;
    client.nbBagages = nbBagages;
    client.PoidsBagages = poidsBagages;

    listesClients.append(client);

    //billet
    ui->billetNom->setText(nom);
    ui->billetPrenom->setText(prenom);
    ui->billetCIN->setText(cin);
    ui->billetEmail->setText(email);
    ui->billettTel->setText(contact);
    ui->billetNbPlace->setText(QString::number(nbPersonne)); // pour recuperer et utiliser le contenu d'un spinBox

    sauvegarderClient(client);
}



double MainWindow::getPrixCategorie(const QString& categorie) {
    if(categorie == "LITE") return client.nbPlaceReserver *25000;
    if(categorie == "PREMIUM") return client.nbPlaceReserver *45000;
    if(categorie == "VIP") return  client.nbPlaceReserver *76000;

    return 0;
}


//SLOT UNIQUE CATEGORIE BUS
void MainWindow::onCategorieClicked(QAbstractButton* button)
{
    QString categorieChoisie = button->text();

    client.categorieClient = categorieChoisie;

    client.prix =getPrixCategorie(categorieChoisie);

    ui->labelAPayer->setText(QString::number(client.prix, 'f', 0) + "Ar");

    ui->labelMonnaie->clear();

    double montant = ui->doubleSpinBoxEspeces->value();

    if(montant > 0) {
        calculPaiment(montant);
    }

    sauvegarderClient(client);
}


void MainWindow::calculPaiment(double montant)
{
    ui->billetMontant->setText(QString::number(client.prix, 'f', 0) + "Ar");
    ui->billetEspece->setText(QString::number(montant, 'f', 0) + "Ar");

    if(montant < client.prix ) {
        QMessageBox::warning(this, "Erreur", "La somme que vous avez payer n'est pas suffisant");
        ui->labelMonnaie->clear();
    } else {
         double rendu = montant - client.prix;
        ui->labelMonnaie->setText(QString::number(rendu, 'f', 0) + "Ar");
        ui->billetMonnaie->setText(QString::number(rendu, 'f', 0) + "Ar");
    }
}


void MainWindow::on_pushButtonValider2_clicked()
{
    qDebug() << "valider2 called, lastResId:" << lastResId;
    double montantPaye = ui->doubleSpinBoxEspeces->value();

    calculPaiment(montantPaye);

    if(montantPaye > client.prix){
        // Update the reservation with payment info
        client.especes = montantPaye;
        client.monnaie = montantPaye - client.prix;

        QSqlQuery qUpdate;
        qUpdate.prepare("UPDATE reservation SET especes = :e, monnaie = :m WHERE id = :id");
        qUpdate.bindValue(":e", client.especes);
        qUpdate.bindValue(":m", client.monnaie);
        qUpdate.bindValue(":id", lastResId);

        if(!qUpdate.exec()) {
            QMessageBox::warning(this,"Erreur","Erreur lors de la mise a jour de la reservation: " + qUpdate.lastError().text());
            return;
        }

        if(lastResId == 0) {
            QMessageBox::warning(this,"Erreur","ID de reservation invalide");
            return;
        }

        // Insérer les places sélectionnées
        for(int numPlace : client.places) {
            QSqlQuery qPlace;
            qPlace.prepare("INSERT INTO reservation_place (reservation_id, numPlace) VALUES (:rid, :np)");
            qPlace.bindValue(":rid", lastResId);
            qPlace.bindValue(":np", numPlace);
            if(!qPlace.exec()) {
                QMessageBox::warning(this,"Erreur","Erreur lors de l'insertion de la place " + QString::number(numPlace) + ": " + qPlace.lastError().text());
            }
        }

        // Verrouiller immédiatement les boutons pour les places réservées
        for(int numPlace : client.places) {
            if(numPlace >= 1 && numPlace <= place->buttons().size()) {
                QAbstractButton* btn = place->buttons().at(numPlace - 1);
                if(btn) {
                    btn->setChecked(false);
                    btn->setEnabled(false);
                    btn->setIcon(iconSelected);
                    btn->setIconSize(QSize(24,24));
                }
            }
        }

        // mettre à jour l'affichage des places
        mettreAJourplaces();

        changerTab2();
    }else {
        QMessageBox::warning(this,"Erreur","Veuillez payer la somme indiquer svp");
    }
}

//affichage place deja occupee
bool MainWindow::estPlaceLibre(
    const QString& villeDepart,
    const QString& villeArrivee,
    const QString& date,
    const QString& heure,
    int numPlace
    ) {

    qDebug() << "estPlaceLibre called for place" << numPlace << "vd:" << villeDepart << "va:" << villeArrivee << "date:" << date << "heure:" << heure;

    if(!QSqlDatabase::database().isOpen()) {
        qDebug() << "DB not open in estPlaceLibre";
        return true; // assume free
    }

    QSqlQuery q;

    q.prepare(
        "SELECT COUNT(*)"
        "FROM reservation r"
        "JOIN reservation_place rp ON r.id = rp.reservation_id"
        "WHERE r.villeDepart = :vd"
        "AND r.villeArrivee = :va"
        "AND r.dateReservation = :d"
        "AND r.heureDepart = :h"
        "AND rp.numPlace = :n"
        );

    q.bindValue(":vd",villeDepart);
    q.bindValue(":va",villeArrivee);
    q.bindValue(":d",date);
    q.bindValue(":h",heure);
    q.bindValue(":n",numPlace);

    if(!q.exec()) {
        qDebug() << "Erreur SQL estPlaceLibre:" << q.lastError().text() << "for place" << numPlace;
        return true; // assume free
    }

    q.next();
    int count = q.value(0).toInt();
    qDebug() << "Count for place" << numPlace << ":" << count;

    bool result = count == 0;
    qDebug() << "estPlaceLibre result:" << result;
    return result; //true = libre
}


QList<int> MainWindow::getReservedPlaces(const QString& villeDepart, const QString& villeArrivee, const QString& date, const QString& heure) {
    QList<int> reserved;
    if(!QSqlDatabase::database().isOpen()) {
        qDebug() << "DB not open in getReservedPlaces";
        return reserved;
    }

    QSqlQuery q;
    q.prepare(
        "SELECT rp.numPlace "
        "FROM reservation r "
        "JOIN reservation_place rp ON r.id = rp.reservation_id "
        "WHERE r.villeDepart = :vd "
        "AND r.villeArrivee = :va "
        "AND r.dateReservation = :d "
        "AND r.heureDepart = :h"
    );
    q.bindValue(":vd", villeDepart);
    q.bindValue(":va", villeArrivee);
    q.bindValue(":d", date);
    q.bindValue(":h", heure);

    if(!q.exec()) {
        qDebug() << "Erreur SQL getReservedPlaces:" << q.lastError().text();
        return reserved;
    }

    while(q.next()) {
        reserved.append(q.value(0).toInt());
    }

    return reserved;
}


/*int MainWindow::getAvailableSeats(const QString& villeDepart, const QString& villeArrivee, const QString& date, const QString& heure)
{
    int totalSeats = 20; // bus a 20 places
    QList<int> reserved = getReservedPlaces(villeDepart, villeArrivee, date, heure);
    int used = reserved.size();
    int avail = totalSeats - used;
    return avail < 0 ? 0 : avail;
}*/


void MainWindow::mettreAJourplaces()
{
    qDebug() << "mettreAJourplaces called";
    QString vd = client.villeDepart;
    QString va = client.villeArivee;
    QString date = client.dateReservation;
    QString heure = client.heureDepart;
    qDebug() << "vd:" << vd << "va:" << va << "date:" << date << "heure:" << heure;

    // récupérer les places réservées depuis la BD et les mettre en set pour accès rapide
    QList<int> reserved = getReservedPlaces(vd, va, date, heure);
    QSet<int> reservedSet;
    for(int r : reserved) reservedSet.insert(r);

    int total = place->buttons().size();
    for(int num = 1; num <= total; ++num) {
        QAbstractButton* btn = place->buttons().at(num - 1);  // Suppose que les boutons sont dans l'ordre 1 à 20
        if(reservedSet.contains(num)) {
            btn->setEnabled(false);
            btn->setChecked(false);
            btn->setIcon(iconSelected);
            btn->setIconSize(QSize(24,24));
        } else {
            btn->setEnabled(true);
            if(btn->isChecked()) {
                btn->setIcon(iconSelected);
            } else {
                btn->setIcon(iconFree);
            }
            btn->setIconSize(QSize(24,24));
        }
    }
}


//SLOT UNIQUE NUMERO DE PLACE
void MainWindow::onSetPlaceClicked(QAbstractButton* button)
{

    int max = client.nbPlaceReserver;
    int selectionees = 0;

    for(QAbstractButton* btn : place->buttons()) {
        if(btn->isChecked())
            selectionees++;
    }


    if(selectionees > max) {
        button->setChecked(false);
        QMessageBox::warning(this,"Limite atteinte", QString("Vous avez reserve %1 place(s) seulement.").arg(max));

        return;
    }

    button->setIcon(button->isChecked() ? iconSelected : iconFree);

    button->setIconSize(QSize(24,24));

    //Mis a jour des places selectionnees
    QStringList placeSelectionnees;
    client.places.clear();  // Vider la liste avant de la remplir

    for (QAbstractButton * btn : place->buttons()) {
        if(btn->isChecked()) {
            placeSelectionnees << btn->text();
            client.places << btn->text().toInt();  // Ajouter le numéro de place à la liste
        }
    }

    ui->billetPlace->setText(placeSelectionnees.join(", "));

    mettreAJourplaces();

}


//sauvegarde dans bd
void MainWindow::sauvegarderClient(const Client &client) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO client("
        "nom,"
        "prenom,"
        "contact,"
        "cin,"
        "email)"
        " VALUES (:n,:p,:ct,:cin,:em)"
        );

    q.bindValue(":n", client.nom);
    q.bindValue(":p", client.prenom);
    q.bindValue(":ct", client.contact);
    q.bindValue(":cin", client.cin);
    q.bindValue(":em", client.email);

    if(!q.exec()) {
        QMessageBox::warning(this,"Erreur","Erreur lors de l'insertion du client: " + q.lastError().text());
        qDebug() << q.lastQuery();
        return;  // Sortir si échec
    }

    int clientId = q.lastInsertId().toInt();  // Récupérer l'ID du client inséré

    q.prepare(
        "INSERT INTO reservation("
        "client_id,"
        "villeDepart,"
        "villeArrivee,"
        "dateReservation,"
        "heureDepart,"
        "nbPlaceReservees,"
        "nbBagages,"
        "poidsBagages,"
        "categorie,"
        "prix,"
        "especes,"
        "monnaie)"
        " VALUES (?,?,?,?,?,?,?,?,?,?,?,?)"
        );

    q.bindValue(0, clientId);
    q.bindValue(1, client.villeDepart);
    q.bindValue(2, client.villeArivee);
    q.bindValue(3, client.dateReservation);
    q.bindValue(4, client.heureDepart);
    q.bindValue(5, client.nbPlaceReserver);
    q.bindValue(6, client.nbBagages);
    q.bindValue(7, client.PoidsBagages);
    q.bindValue(8, client.categorieClient);
    q.bindValue(9, client.prix);
    q.bindValue(10, client.especes);
    q.bindValue(11, client.monnaie);


    if(!q.exec()) {
        QMessageBox::warning(this,"Erreur","Erreur lors de l'insertion de la reservation: " + q.lastError().text());
        qDebug() << q.lastQuery();
        return;  // Sortir si échec
    }

    lastResId = q.lastInsertId().toInt();  // Stocker l'ID de la réservation

    qDebug() << "lastResId set to:" << lastResId;

    int resId = q.lastInsertId().toInt();  // Récupérer l'ID de la réservation

}


