#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    socketDeDialogueAvecServeur=new QTcpSocket();

    connect(socketDeDialogueAvecServeur,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this,
            &ClientWidget::onQTcpSocket_error);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::stateChanged,this,&ClientWidget::onQTcpSocket_stateChanged);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::connected,this,&ClientWidget::onQTcpSocket_connected);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::disconnected,this,&ClientWidget::onQTcpSocket_disconnected);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::hostFound,this,&ClientWidget::onQTcpSocket_hostFound);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::aboutToClose,this,&ClientWidget::onQTcpSocket_aboutToClose);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::bytesWritten,this,&ClientWidget::onQTcpSocket_bytesWritten);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::readChannelFinished,this,&ClientWidget::onQTcpSocket_readChannelFinished);
    connect(socketDeDialogueAvecServeur,&QTcpSocket::readyRead,this,&ClientWidget::onQTcpSocket_readyRead);
    ui->groupBoxClient->hide();


}

ClientWidget::~ClientWidget()
{
    delete ui;
    delete socketDeDialogueAvecServeur;
}


void ClientWidget::on_pushButtonConnexion_clicked()
{
    if(ui->pushButtonConnexion->text()=="Connexion"){
        QHostAddress adresse(ui->lineEditAdresse->text());
        socketDeDialogueAvecServeur->connectToHost(adresse,ui->lineEditPort->text().toUInt());
        ui->pushButtonConnexion->setText("Deconnection");
    }else{
        socketDeDialogueAvecServeur->disconnectFromHost();
        ui->pushButtonConnexion->setText("Connexion");
    }

}

void ClientWidget::on_pushButtonNom_clicked()
{
    typeDeDemande="c";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}

void ClientWidget::on_pushButtonArchitecture_clicked()
{
    typeDeDemande="a";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}

void ClientWidget::on_pushButtonOS_clicked()
{
    typeDeDemande="o";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}

void ClientWidget::on_pushButtonNomUtilisateur_clicked()
{
    typeDeDemande="u";
    socketDeDialogueAvecServeur->write(typeDeDemande.toLatin1());
}

void ClientWidget::onQTcpSocket_connected()
{

    ui->textEditEtat->append("Connection");
    ui->groupBoxClient->show();
    ui->lineEditAdresse->setEnabled(false);
    ui->lineEditPort->setEnabled(false);


}

void ClientWidget::onQTcpSocket_disconnected()
{
    ui->textEditEtat->append("Déconnection");
    ui->groupBoxClient->hide();
    ui->lineEditAdresse->setEnabled(true);
    ui->lineEditPort->setEnabled(true);
}

void ClientWidget::onQTcpSocket_error(QAbstractSocket::SocketError socketError)
{
    ui->textEditEtat->append(socketDeDialogueAvecServeur->errorString());
}

void ClientWidget::onQTcpSocket_hostFound()
{
    ui->textEditEtat->append("Hote trouvé");
}

void ClientWidget::onQTcpSocket_stateChanged(QAbstractSocket::SocketState socketState)
{
    ui->textEditEtat->append("Etat changer");
}

void ClientWidget::onQTcpSocket_aboutToClose()
{
    ui->textEditEtat->append("Va ce fermer");
}

void ClientWidget::onQTcpSocket_bytesWritten(qint64 bytes)
{
    ui->textEditEtat->append("Ecriture");
}

void ClientWidget::onQTcpSocket_readChannelFinished()
{
    ui->textEditEtat->append("Lecture de la file de lecture terminer");


}


void ClientWidget::onQTcpSocket_readyRead()
{
    ui->textEditEtat->append("Lecture pret");
    QByteArray data= socketDeDialogueAvecServeur->readAll();
    qDebug()<<typeDeDemande.at(0).toLatin1();

    switch (typeDeDemande.at(0).toLatin1()) {
    case 'u':
        ui->lineEditNomUtilisateur->setText(data);
         break;
    case 'c':
        ui->lineEditNom->setText(data);
        break;
    case 'o':
        ui->lineEditOS->setText(data);
        break;
    case 'a':
        ui->lineEditArchitecture->setText(data);
        break;
    default:
        break;
    }
}
