#include "serveurwidget.h"
#include "ui_serveurwidget.h"

Serveurwidget::Serveurwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Serveurwidget)
    ,socketEcoutServeur(new QTcpServer)


{
    ui->setupUi(this);



    if(!socketEcoutServeur->listen(QHostAddress::Any,ui->spinBoxPort->value())){
        ui->listWidget->addItem("erreur listen"+socketEcoutServeur->errorString());
    }else
        ui->listWidget->addItem("Le serveur est démarré");
    connect(socketEcoutServeur,&QTcpServer::newConnection,this,&Serveurwidget::onQTcpServer_newConnection);
    ui->listWidget->hide();


}

Serveurwidget::~Serveurwidget()
{
    delete ui;
    delete socketEcoutServeur;
}


void Serveurwidget::on_pushButton_clicked()
{

}

void Serveurwidget::on_pushButtonLancement_clicked()
{
    if(ui->pushButtonLancement->text()=="Lancement serveur"){
        ui->listWidget->show();
    }else{
        ui->listWidget->hide();
    }
}
void Serveurwidget::onQTcpServer_newConnection()
{
    QTcpSocket *client;
    QProcess *leProcess;
    leProcess =new QProcess;
    client=socketEcoutServeur->nextPendingConnection();
    connect(client,&QTcpSocket::readyRead,this,&Serveurwidget::onQtcpSocket_readyRead);
    connect(client,&QTcpSocket::disconnected,this,&Serveurwidget::onQtpSocket_disconnected);
    socketDialogueClient.append(client);
    connect(leProcess,&QProcess::readyReadStandardOutput,this,&Serveurwidget::onQProcess_readyReadStandardOutput);
    process.append(leProcess);
}

void Serveurwidget::onQtcpSocket_readyRead()
{

    ui->listWidget->addItem("Pret a lire");
    QTcpSocket *client=qobject_cast<QTcpSocket*>(sender());
    int indexProcess=socketDialogueClient.indexOf(client);



    QChar commande;
    if(socketDialogueClient.at(indexProcess)->bytesAvailable()){


        QByteArray tmp=socketDialogueClient.at(indexProcess)->readAll();
        commande=tmp.at(0);
        QString messageCommande = " Commande du client "+ tmp+"";
        ui->listWidget->addItem(messageCommande);
        QString reponse;
        QHostAddress adresse=socketDialogueClient.at(indexProcess)->peerAddress();

        switch(commande.toLatin1()){
        case'u':

            reponse = getenv("USER");

            socketDialogueClient.at(indexProcess)->write(reponse.toUtf8());
            ui->listWidget->addItem(adresse.toString());



            break;

        case'c':

            reponse=QSysInfo::machineHostName();
            socketDialogueClient.at(indexProcess)->write(reponse.toUtf8());
            ui->listWidget->addItem(adresse.toString());

            break;
        case'o':

            reponse=QSysInfo::kernelType();
            socketDialogueClient.at(indexProcess)->write(reponse.toUtf8());
            ui->listWidget->addItem(adresse.toString());

            break;
        case'a':

            reponse=QSysInfo::currentCpuArchitecture();
            socketDialogueClient.at(indexProcess)->write(reponse.toUtf8());
            ui->listWidget->addItem(adresse.toString());

            break;

        }



    }
}

void Serveurwidget::onQtpSocket_disconnected()
{
    QTcpSocket *client=qobject_cast<QTcpSocket*>(sender());
   int indexProcess=socketDialogueClient.indexOf(client);
   QHostAddress adresse=socketDialogueClient.at(indexProcess)->localAddress();
     socketDialogueClient.removeAt(indexProcess);
     process.removeAt(indexProcess);
     ui->listWidget->addItem("Le client "+adresse.toString()+" c'est déconnecter");

}

void Serveurwidget::onQProcess_readyReadStandardOutput()
{
    QTcpSocket*client=qobject_cast<QTcpSocket*>(sender());
    int indexProcess=socketDialogueClient.indexOf(client);
    QString reponse=process.at(indexProcess)->readAllStandardOutput();
    if(!reponse.isEmpty()){
        QString message="Réponse envoyée à"  +  socketDialogueClient.at(indexProcess)->peerAddress().toString()+":"+reponse;
        ui->listWidget->addItem(message);
        socketDialogueClient.at(indexProcess)->write(reponse.toLatin1());}
}
