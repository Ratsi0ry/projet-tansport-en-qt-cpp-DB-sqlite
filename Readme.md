# projet transport - QT & C++
Bienvenue dans **Projet Transport**, une application Qt/C++ pour gerer les **reservations de clients**, **les places**, et les **suivi des bagages**.

Un projet simple mais efficace pour apprendre **Qt**, **Sqlite** et le **developpement d'interface moderne**.

---
## 💡Fonctionnalites
- ajouter/gerer des clients avec:nom, prenom, contact, CIN, email, ville de depart & arrivee, date et heure de reservation categorie du client, num place, nombre deplace a reserver, bagages,prix, payment en especes et monnaie
- **sauvegarde automatique dan sqlite**
- interface simple et reactive grace a QT Widgets

## 🛠️ Installation & compilation

### Prerequis
- **Qt6.x ou 5.x**
- **CMAKE 3.20+**
- **Compiler C++ moderne (MSVCA / MinGw/ Gcc)

###Compilation
1. Clone le projet:
   ```bash git clone https://github.com/Ratsi0ry/projet-tansport-en-qt-cpp-DB-sqlite.git cd NOM_DU_REPO
2. Cree le dossier build et compile:
   mkdir build
   cd build
   cmake ..
   cmake --build .
3.Lance l'application:
./TransportAPp # ou TransportApp.exe sur Windows   
