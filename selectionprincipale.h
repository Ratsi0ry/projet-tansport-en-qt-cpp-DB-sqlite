#ifndef SELECTIONPRINCIPALE_H
#define SELECTIONPRINCIPALE_H

#include <QString>
#include <vector>
using namespace std;

struct InfoTransport {

};

class selectionPrincipale
{
private:
    QString dateDepart;
    QString heureDepart;
    QString villeDepart;
    QString villeArrivee;

public:
    selectionPrincipale();

    void getDateDepart() const;
    void getHeureDepart() const;
    void getVilleDepart() const;
    void getVilleArivee() const;
};

#endif // SELECTIONPRINCIPALE_H
