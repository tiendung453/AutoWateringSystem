#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <Qstring>
#include <QDebug>
#include <QLCDNumber>


QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QStringList ConnectedDevices();

    bool Connect(QString Port, uint32_t bd);
    bool Disconnect(void);

private slots:
    void on_pushButtonConnect_clicked();
    void on_pushButtonDisconnect_clicked();
    void ReadData();
    qint64 WriteData(const char *cmd);


    void on_pushButtonAuto_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButtonExit_clicked();

    void on_pushButtonOpenRoof_clicked();

    void on_pushButtonCloseRoof_clicked();

    void on_pushButtonOnPump_clicked();

    void on_pushButtonOffPump_clicked();

private:
    Ui::Dialog *ui;
    QSerialPort *COMPORT;
    QLCDNumber *lcdTemperature;
    QLCDNumber *lcdHumidity;
    QLCDNumber *lcdSoilmoisture;
};
#endif // DIALOG_H
