#ifndef SERVEURWIDGET_H
#define SERVEURWIDGET_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostInfo>
#include <QProcess>


QT_BEGIN_NAMESPACE
namespace Ui { class ServeurWidget; }
QT_END_NAMESPACE

class ServeurWidget : public QMainWindow
{
    Q_OBJECT

public:
    ServeurWidget(QWidget *parent = nullptr);
    ~ServeurWidget();

private slots:
    void on_pushButtonLancement_clicked();
    void onQTcpServer_newConnection();//sera appelée en cas d'une demandede connexion d'un client
    void onQtcpSocket_readyRead();
    void onQtpSocket_disconnected();
    void onQProcess_readyReadStandardOutput();

private:
    Ui::ServeurWidget *ui;
    QTcpServer *socketEcoutServeur;
    QTcpSocket *socketDialogueClient;
    QProcess *process;




};
#endif // SERVEURWIDGET_H
