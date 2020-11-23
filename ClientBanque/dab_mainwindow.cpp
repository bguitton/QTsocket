#include "dab_mainwindow.h"
#include "ui_dab_mainwindow.h"

DAB_MainWindow::DAB_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DAB_MainWindow)
{
    ui->setupUi(this);
    ui->groupBoxOperation->setEnabled(false);
    socketClientBanque=new QTcpSocket();

    connect(socketClientBanque,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this,
            &DAB_MainWindow::onQTcpSocket_error);
    connect(socketClientBanque,&QTcpSocket::stateChanged,this,&DAB_MainWindow::onQTcpSocket_stateChanged);
    connect(socketClientBanque,&QTcpSocket::connected,this,&DAB_MainWindow::onQTcpSocket_connected);
    connect(socketClientBanque,&QTcpSocket::disconnected,this,&DAB_MainWindow::onQTcpSocket_disconnected);
    connect(socketClientBanque,&QTcpSocket::hostFound,this,&DAB_MainWindow::onQTcpSocket_hostFound);
    connect(socketClientBanque,&QTcpSocket::aboutToClose,this,&DAB_MainWindow::onQTcpSocket_aboutToClose);
    connect(socketClientBanque,&QTcpSocket::bytesWritten,this,&DAB_MainWindow::onQTcpSocket_bytesWritten);
    connect(socketClientBanque,&QTcpSocket::readChannelFinished,this,&DAB_MainWindow::onQTcpSocket_readChannelFinished);
    connect(socketClientBanque,&QTcpSocket::readyRead,this,&DAB_MainWindow::onQTcpSocket_readyRead);



}

DAB_MainWindow::~DAB_MainWindow()
{
    delete ui;
    delete socketClientBanque;
}


void DAB_MainWindow::on_pushButtonConnexion_clicked()
{
    if(ui->pushButtonConnexion->text()=="Connexion"){
       QHostAddress adresse(ui->lineEditAdresse->text());
       socketClientBanque->connectToHost(adresse,ui->lineEditPort->text().toUInt());
        ui->pushButtonConnexion->setText("Deconnection");
        ui->groupBoxOperation->setEnabled(true);
   }else{
        socketClientBanque->disconnectFromHost();
        ui->pushButtonConnexion->setText("Connexion");
        ui->groupBoxOperation->setEnabled(false);
    }
}

void DAB_MainWindow::onQTcpSocket_connected()
{
    ui->listWidgetEtat->addItem("Connection");

    ui->lineEditAdresse->setEnabled(false);
    ui->lineEditPort->setEnabled(false);
    ui->lineEditMontant->setEnabled(false);
    ui->radioButtonRetrait->setEnabled(false);
    ui->radioButtonSolde->setEnabled(false);
    ui->radioButtonDepot->setEnabled(false);
    ui->pushButtonEnvoi->setEnabled(false);
}

void DAB_MainWindow::onQTcpSocket_disconnected()
{
    ui->listWidgetEtat->addItem("Déconnection");
   // ui->groupBoxClient->hide();
    ui->lineEditAdresse->setEnabled(true);
    ui->lineEditPort->setEnabled(true);

}
void DAB_MainWindow::onQTcpSocket_error(QAbstractSocket::SocketError socketError)
{
    ui->listWidgetEtat->addItem(socketClientBanque->errorString());
}

void DAB_MainWindow::onQTcpSocket_hostFound()
{
    ui->listWidgetEtat->addItem("Hote trouvé");
}

void DAB_MainWindow::onQTcpSocket_stateChanged(QAbstractSocket::SocketState socketState)
{
    ui->listWidgetEtat->addItem("Etat changer");
}

void DAB_MainWindow::onQTcpSocket_aboutToClose()
{
    ui->listWidgetEtat->addItem("Va ce fermer");
}

void DAB_MainWindow::onQTcpSocket_bytesWritten(qint64 bytes)
{
    ui->listWidgetEtat->addItem("Ecriture");
}

void DAB_MainWindow::onQTcpSocket_readChannelFinished()
{
    ui->listWidgetEtat->addItem("Lecture de la file de lecture terminer");


}

void DAB_MainWindow::onQTcpSocket_readyRead()
{
quint16 taille=0;
QString message;
// si le nombre d'octets recu est au moin egal a celui de la taille de ce que l'on doit recevoir
if(socketClientBanque->bytesAvailable()>=(qint64)sizeof (taille)){
    //association de la socket au flux d'entree
    QDataStream in(socketClientBanque);
    // extraire la taille de ce que l'on doit recevoir
    in>>taille;
    //si le nombre d'octets recu est au moins egal a celui de ce l'on doit recevoir
    if(socketClientBanque->bytesAvailable()>=(quint64)taille){
        //extraire le message de la banque et le mettre dans message
        in>>message;


    }
}




ui->lineEditMessage->setText(message);

}

void DAB_MainWindow::on_pushButtonEnvoi_clicked()
{
    QChar commande;
    quint16 taille=0;
    QBuffer tampon;


    float val =ui->lineEditMontant->text().toFloat();
    if(val>0){
    if(ui->radioButtonDepot->isChecked()){
        commande='D';
        tampon.open(QIODevice::WriteOnly);
        // association du tampon au flux de sortie
        QDataStream out(&tampon);
        // construction de la trame
        out<<taille<<commande<<val;
        //calcule de la taille de la trame
        taille=tampon.size()-sizeof (taille);
        //placement sur la premiere position du flux pour pouvoir modifier la taille
        tampon.seek(0);
        //modification de la trame avec la taille real de la trame
        out<<taille;
        socketClientBanque->write(tampon.buffer());
    }
    if(ui->radioButtonRetrait->isChecked()){
        commande='R';
        tampon.open(QIODevice::WriteOnly);
        // association du tampon au flux de sortie
        QDataStream out(&tampon);
        // construction de la trame
        out<<taille<<commande<<val;
        //calcule de la taille de la trame
        taille=tampon.size()-sizeof (taille);
        //placement sur la premiere position du flux pour pouvoir modifier la taille
        tampon.seek(0);
        //modification de la trame avec la taille real de la trame
        out<<taille;
        socketClientBanque->write(tampon.buffer());
    }
    if(ui->radioButtonSolde->isChecked()){
        commande='S';
        tampon.open(QIODevice::WriteOnly);
        // association du tampon au flux de sortie
        QDataStream out(&tampon);
        // construction de la trame
        out<<taille<<commande;
        //calcule de la taille de la trame
        taille=tampon.size()-sizeof (taille);
        //placement sur la premiere position du flux pour pouvoir modifier la taille
        tampon.seek(0);
        //modification de la trame avec la taille real de la trame
        out<<taille;
        socketClientBanque->write(tampon.buffer());
    }
}else
        ui->lineEditMessage->setText("le chiffre et négatife ou n'est pas un nombre");






}


void DAB_MainWindow::on_pushButtonCompte_clicked()
{
    quint16 taille=0;
    QChar commande='N';
    int compte=1234;
    QBuffer tampon;
    tampon.open(QIODevice::WriteOnly);
    // association du tampon au flux de sortie
    QDataStream out(&tampon);
    // construction de la trame
    out<<taille<<commande<<compte;
    //calcule de la taille de la trame
    taille=tampon.size()-sizeof (taille);
    //placement sur la premiere position du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille real de la trame
    out<<taille;
    socketClientBanque->write(tampon.buffer());
    QString message;
    // si le nombre d'octets recu est au moin egal a celui de la taille de ce que l'on doit recevoir
    if(socketClientBanque->bytesAvailable()>=(qint64)sizeof (taille)){
        //association de la socket au flux d'entree
        QDataStream in(socketClientBanque);
        // extraire la taille de ce que l'on doit recevoir
        in>>taille;
        //si le nombre d'octets recu est au moins egal a celui de ce l'on doit recevoir
        if(socketClientBanque->bytesAvailable()>=(quint64)taille){
            //extraire le message de la banque et le mettre dans message
            in>>message;
ui->lineEditMessage->setText(message);

        }
       // ui->lineEditMessage->setText(message);

}
    ui->lineEditMontant->setEnabled(true);
    ui->radioButtonRetrait->setEnabled(true);
    ui->radioButtonSolde->setEnabled(true);
    ui->radioButtonDepot->setEnabled(true);
    ui->pushButtonEnvoi->setEnabled(true);

}
