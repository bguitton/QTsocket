#ifndef DAB_MAINWINDOW_H
#define DAB_MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QChar>
#include <QBuffer>
#include <QDataStream>
#include <QValidator>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class DAB_MainWindow; }
QT_END_NAMESPACE

class DAB_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    DAB_MainWindow(QWidget *parent = nullptr);
    ~DAB_MainWindow();

private slots:
    void on_pushButtonConnexion_clicked();

    void onQTcpSocket_connected();

    void onQTcpSocket_disconnected();

    void onQTcpSocket_error(QAbstractSocket::SocketError socketError);

    void onQTcpSocket_hostFound();

    void onQTcpSocket_stateChanged(QAbstractSocket::SocketState socketState);

    void onQTcpSocket_aboutToClose();

    void onQTcpSocket_bytesWritten(qint64 bytes);

    void onQTcpSocket_readChannelFinished();

    void onQTcpSocket_readyRead();


    void on_pushButtonEnvoi_clicked();

    void on_pushButtonCompte_clicked();

private:
    Ui::DAB_MainWindow *ui;
    QTcpSocket *socketClientBanque;
};
#endif // DAB_MAINWINDOW_H
