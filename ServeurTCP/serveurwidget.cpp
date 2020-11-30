#include "serveurwidget.h"
#include "ui_serveurwidget.h"
#include <QtDebug>

ServeurWidget::ServeurWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServeurWidget)
    ,socketEcoutServeur(new QTcpServer)
    ,socketDialogueClient(nullptr)
    ,process(new QProcess)

{
    ui->setupUi(this);
    if(!socketEcoutServeur->listen(QHostAddress::Any,ui->spinBoxPort->value())){
        ui->listWidget->addItem("erreur listen"+socketEcoutServeur->errorString());
    }else
        ui->listWidget->addItem("Le serveur est démarré");
    connect(socketEcoutServeur,&QTcpServer::newConnection,this,&ServeurWidget::onQTcpServer_newConnection);
    ui->listWidget->hide();


}

ServeurWidget::~ServeurWidget()
{
    delete ui;
    delete socketEcoutServeur;
    if(socketDialogueClient!=nullptr)
        delete socketDialogueClient;

}


void ServeurWidget::on_pushButtonLancement_clicked()
{
    if(ui->pushButtonLancement->text()=="Lancement serveur"){
        ui->listWidget->show();
    }else{
        ui->listWidget->hide();
    }
}
void ServeurWidget::onQTcpServer_newConnection()
{
    ui->pushButtonLancement->setText("Déconnexion serveur");
    socketDialogueClient=socketEcoutServeur->nextPendingConnection();
    ui->listWidget->addItem("Nouvelle connection");

    connect(socketDialogueClient,&QTcpSocket::readyRead,this,&ServeurWidget::onQtcpSocket_readyRead);
    connect(socketDialogueClient,&QTcpSocket::disconnected,this,&ServeurWidget::onQtpSocket_disconnected);

}

void ServeurWidget::onQtcpSocket_readyRead()
{

    ui->listWidget->addItem("Pret a lire");

    QChar commande;
    if(socketDialogueClient->bytesAvailable()){// données disponible ?


        QByteArray tmp=socketDialogueClient->readAll();
        commande=tmp.at(0);
        QString messageCommande = " Commande du client "+ tmp+"";
        ui->listWidget->addItem(messageCommande);
        QString reponse;
        switch(commande.toLatin1()){
        case'u':
            reponse = getenv("USER");

            socketDialogueClient->write(reponse.toUtf8());


            break;

        case'c':

            reponse=QSysInfo::machineHostName();
            socketDialogueClient->write(reponse.toUtf8());
            break;
        case'o':

            reponse=QSysInfo::kernelType();
            socketDialogueClient->write(reponse.toUtf8());
            break;
        case'a':

            reponse=QSysInfo::currentCpuArchitecture();
            socketDialogueClient->write(reponse.toUtf8());
            break;

        }



    }
}

void ServeurWidget::onQtpSocket_disconnected()
{

    delete socketDialogueClient;
}

void ServeurWidget::onQProcess_readyReadStandardOutput()
{
    QString reponse=process->readAllStandardOutput();
    if(!reponse.isEmpty()){
        QString message="Réponse envoyée à"  +  socketDialogueClient->peerAddress().toString()+":"+reponse;
        ui->listWidget->addItem(message);
        socketDialogueClient->write(reponse.toLatin1());}
}
