#include "client.h"

ClientInfo::ClientInfo() {}

const Client& ClientInfo::getDernierClient() const {
    return clients.back();
}


/*void ajouterClient() const(std::string& nom, std::string& prenom, std::string& contact, std::string& cin, std::string& email) {
    Client c;
    c.nom = nom;
    c.prenom = prenom;
    c.contact = contact;
    c.cin = cin;
    c.email = email;

    clients.push_back(c);
}*/


