#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "client.h"

#include <QButtonGroup>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static double getPrixCategorie(const QString& categorie);

private slots:
    void on_pushButtonValider1_clicked();
    void on_pushButtonReserverTanaManj_clicked();
    void changerTab2();
    void on_pushButtonToliFian_clicked();
    void on_pushButtonTanaToam_clicked();
    void on_pushButtonToamDiego_clicked();
    void on_pushButtonReserverClient_clicked();
    void onCategorieClicked(QAbstractButton* button);
    void onSetPlaceClicked(QAbstractButton* button);
    void on_pushButtonValider2_clicked();
    void calculPaiment(double montantPaye);
    void sauvegarderClient(const Client &client);

private:
    QVector<Client> listesClients;
    QButtonGroup* groupeCategories;
    QButtonGroup*  place;
    Ui::MainWindow *ui;

    //pour gerer les places
    QIcon iconFree;
    QIcon iconSelected;
    int lastResId;

    void mettreAJourplaces();
    bool estPlaceLibre(const QString& villeDepart, const QString& villeArrivee, const QString& date, const QString& heure, int numPlace);
    // récupère numéros des places réservées pour un trajet/date/heure
    QList<int> getReservedPlaces(const QString& villeDepart, const QString& villeArrivee, const QString& date, const QString& heure);

};
#endif // MAINWINDOW_H
