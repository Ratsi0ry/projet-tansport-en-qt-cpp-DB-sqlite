#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDate>
#include <QMessageBox>
#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // l'app s'ouvre toujours en premier sur le tab1
    ui->tabWidget->setCurrentIndex(0);

    //date
    QString dateDujour = QDate::currentDate().toString("dd//MM//yyyy");
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

    //BASE DE DONNEES
    qDebug() << "Drivers SQl disponibles:" << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("clients.db");

    if(!db.open()) {
        qDebug() << "Erreur DB:" << db.lastError().text();
    }

    qDebug() << "DB utilisee:" << db.databaseName();
    qDebug() << "DB ouverte?:" << db.isOpen();


    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS client("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "nom TEXT, prenom TEXT, contact TEXT, cin TEXT, email TEXT,"
           "villeDepart TEXT, villeArrivee TEXT,"
           "heureDepart TEXT, dateReservation TEXT,"
           "categorieClient TEXT, numPlace TEXT,"
           "nbPlaceReserver INTEGER, nbBagages INTEGER,"
           "poidsBagages REAl, prix REAL, especes REAl, monnaie REAL)");

}

MainWindow::~MainWindow()
{
    delete ui;
}

Client client; // regroupe les info sur le client

void MainWindow::changerTab2()
{
    int indexActuel = ui->tabWidget->currentIndex();
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

    sauvegarderClient(client);

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
    double montantPaye = ui->doubleSpinBoxEspeces->value();

    calculPaiment(montantPaye);

    if(montantPaye > client.prix){
        changerTab2();
    }else {
        QMessageBox::warning(this,"Erreur","Veuillez payer la somme indiquer svp");
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

    //Mis a jour des places selectionnees
    QStringList placeSelectionnees;

    for (QAbstractButton * btn : place->buttons()) {
        if(btn->isChecked()) {
            placeSelectionnees << btn->text();
        }
    }

    ui->billetPlace->setText(placeSelectionnees.join(", "));
}


//sauvegarde dans bd
void MainWindow::sauvegarderClient(const Client &client) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO client(nom, prenom, contact, cin, email, villeDepart, villeArrivee, heureDepart, dateReservation,categorieClient, numPlace, nbPlaceReserver, nbBagages, poidsBagages, prix, especes, monnaie) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    q.addBindValue(client.nom);
    q.addBindValue(client.prenom);
    q.addBindValue(client.contact);
    q.addBindValue(client.cin);
    q.addBindValue(client.email);
    q.addBindValue(client.villeDepart);
    q.addBindValue(client.villeArivee);
    q.addBindValue(client.heureDepart);
    q.addBindValue(client.dateReservation);
    q.addBindValue(client.categorieClient);
    q.addBindValue(client.numPlace);
    q.addBindValue(client.nbPlaceReserver);
    q.addBindValue(client.nbBagages);
    q.addBindValue(client.PoidsBagages);
    q.addBindValue(client.prix);
    q.addBindValue(client.especes);
    q.addBindValue(client.monnaie);

    if(!q.exec()) {
        qDebug() << "INSERT FAILED";
        qDebug() << q.lastError().text();
        qDebug() << q.lastQuery();
    }
}


