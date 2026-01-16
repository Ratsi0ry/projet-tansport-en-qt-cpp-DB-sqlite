#ifndef CLIENT_H
#define CLIENT_H

#include<QString>
#include<QList>
using namespace std;

struct Client{
    QString nom;
    QString prenom;
    QString contact;
    QString cin;
    QString email;
    QString villeDepart;
    QString villeArivee;
    QString heureDepart;
    QString dateReservation;
    QString categorieClient;
    QList<int> places;  // Liste des numéros de places sélectionnées
    int nbPlaceReserver;
    int nbBagages;
    double PoidsBagages;
    double prix;
    double especes;
    double monnaie;

};

#endif // CLIENT_H
