#ifndef SERVEURWIDGET_H
#define SERVEURWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostInfo>
#include <QProcess>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class Serveurwidget; }
QT_END_NAMESPACE

class Serveurwidget : public QWidget
{
    Q_OBJECT

public:
    Serveurwidget(QWidget *parent = nullptr);
    ~Serveurwidget();

private slots:
    void on_pushButton_clicked();

    void onQTcpServer_newConnection();//sera appelée en cas d'une demandede connexion d'un client
    void onQtcpSocket_readyRead();
    void onQtpSocket_disconnected();
    void onQProcess_readyReadStandardOutput();

    void on_pushButtonLancement_clicked();

private:
    Ui::Serveurwidget *ui;
    QTcpServer *socketEcoutServeur;
    QList<QTcpSocket*> socketDialogueClient;
    QList<QProcess*> process;
};
#endif // SERVEURWIDGET_H
