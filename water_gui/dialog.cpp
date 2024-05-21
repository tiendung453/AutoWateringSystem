#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    COMPORT = new QSerialPort(this);
    QStringList DispSerial = this->ConnectedDevices();
    // Thêm cổng Port có sẵn vào comboBoxPort
    for(int i=0; i<DispSerial.length(); i+=1){
        ui->comboBoxPort->addItem(DispSerial[i]);
    }

    if(DispSerial.length() > 0){
        ui->pushButtonConnect->setEnabled(true);
        ui->textEditGetData->append("### Serial port ready for use.");
    }
    else{
        ui->textEditGetData->append("### No serial ports were detected!");
    }
    // Thêm BaudRate vào comboBoxBaudRate
    ui->comboBoxBaudRate->addItem("2400");
    ui->comboBoxBaudRate->addItem("4800");
    ui->comboBoxBaudRate->addItem("9600");
    ui->comboBoxBaudRate->addItem("19200");
    ui->comboBoxBaudRate->addItem("115200");
}

Dialog::~Dialog()
{
    COMPORT->clear();  // xóa dữ liệu đang chờ trong bộ đệm (buffer) của cổng Serial
    COMPORT->close();  // đóng cổng Serial
    delete ui;
}

// Lấy danh sách các cổng Serial có sẵn trên thiết bị
QStringList Dialog::ConnectedDevices()
{
    QStringList devs;

    foreach(const QSerialPortInfo info, QSerialPortInfo::availablePorts()){
        COMPORT->setPort(info);
        if(COMPORT->open(QIODevice::ReadWrite)){
            COMPORT->close();
            devs << info.portName();
        }
    }
    return devs;
}

// Kết nổi cổng Serial
bool Dialog::Connect(QString Port, uint32_t bd)
{
    COMPORT->setPortName(Port);
    qDebug() << "Serial port device: " << Port;

    switch(bd){
    case 2400:
        qDebug() << "Baudrate: 2400";
        COMPORT->setBaudRate(QSerialPort::Baud2400);
        break;
    case 4800:
        qDebug() << "Baudrate: 4800";
        COMPORT->setBaudRate(QSerialPort::Baud4800);
        break;
    case 9600:
        qDebug() << "Baudrate: 9600";
        COMPORT->setBaudRate(QSerialPort::Baud9600);
        break;
    case 19200:
        qDebug() << "Baudrate: 19200";
        COMPORT->setBaudRate(QSerialPort::Baud19200);
        break;
    case 115200:
        qDebug() << "Baudrate: 115200";
        COMPORT->setBaudRate(QSerialPort::Baud115200);
        break;
    }

    COMPORT->setFlowControl(QSerialPort::NoFlowControl);
    COMPORT->setDataBits(QSerialPort::Data8);
    COMPORT->setParity(QSerialPort::NoParity);
    COMPORT->setStopBits(QSerialPort::OneStop);
    COMPORT->open(QIODevice::ReadWrite);

    if(COMPORT->isOpen()){
        qDebug() << "Seccessfully opened serial port!";
        return true;
    }
    else{
        qDebug() << "Failed to open serial port!";
        qDebug() << "Error: " << COMPORT->error();
        return false;
    }
}

// Ngắt kết nối cổng Serial
bool Dialog::Disconnect()
{
    COMPORT->clear();
    COMPORT->close();

    if(COMPORT->error() == 0 || !COMPORT->isOpen())
    {
        qDebug() << "Successfully closed serial port!";
        return true;
    }
    else
    {
        qDebug() << "Failed to close serial port! ERROR: " << COMPORT->error();
        return false;
    }
}

void Dialog::on_pushButtonConnect_clicked()
{
    bool statusOpenSerial;
    statusOpenSerial = this->Connect(ui->comboBoxPort->currentText(), ui->comboBoxBaudRate->currentText().toInt());

    if(statusOpenSerial)
    {
        ui->pushButtonDisconnect->setEnabled(true);
        ui->pushButtonConnect->setEnabled(false);
        ui->textEditGetData->append("### Successfully opened serial port!");
        QObject::connect(COMPORT, SIGNAL(readyRead()), this, SLOT(ReadData()));
    }
    else{
        ui->textEditGetData->append("### Failed to open serial connection.");
    }
}


void Dialog::on_pushButtonDisconnect_clicked()
{
    bool statusCloseSerial;
    statusCloseSerial = this->Disconnect();

    if(statusCloseSerial){
        ui->pushButtonDisconnect->setEnabled(false);
        ui->pushButtonConnect->setEnabled(true);
        ui->textEditGetData->append("### Successfully closed serial port!");
    }
    else
    {
        ui->textEditGetData->append("### Failed to close serial connection.");
    }
}

// Gửi dữ liệu lên bộ đệm của cổng Serial
qint64 Dialog::WriteData(const char *cmd)
{
    qint64 sizeWritten;
    sizeWritten = COMPORT->write(cmd,qstrlen(cmd)); // Trả về số lượng byte thực sự đã được gửi đi
    if (sizeWritten > 0) {
        if (COMPORT->waitForBytesWritten())
        {
            // Tất cả các byte đã được ghi thành công
        } else {
            // Quá trình ghi không thành công hoặc hết thời gian chờ đợi
            qDebug() << "Ghi không thành công hoặc hết thời gian chờ đợi!";
        }
    } else {
        // Quá trình ghi không thành công
        qDebug() << "Ghi không thành công!";
    }
    return sizeWritten;
}

// Đọc dữ liệu có sẵn trong bộ đệm của cổng Serial
void Dialog::ReadData(){
    while (COMPORT->canReadLine())
    {
        QByteArray data = COMPORT->readLine().trimmed();
        QList<QByteArray> values = data.split(',');
        qDebug()<< values.size();

        if (values.size() == 3)
        {
            bool tempOk, humiOk, moistureOk;
            double temp = values[0].toDouble(&tempOk);
            double humi = values[1].toDouble(&humiOk);
            int moisture = values[2].toInt(&moistureOk);

            if (tempOk)
            {
                ui->lcdTemperature->display(temp);
            }
            if (humiOk)
            {
                ui->lcdHumidity->display(humi);
            }
            if (moistureOk)
            {
                ui->lcdSoilmoisture->display(moisture);
            }
        }
    }
}


void Dialog::on_pushButtonExit_clicked()
{
    COMPORT->close();
    this->close();
}


void Dialog::on_pushButtonAuto_clicked()
{
    QByteArray requestDataToSend = "1";
    this->WriteData(requestDataToSend);
}


void Dialog::on_pushButtonStop_clicked()
{
    QByteArray requestDataToSend = "0";
    this->WriteData(requestDataToSend);
}


void Dialog::on_pushButtonOpenRoof_clicked()
{
    QByteArray requestDataToSend = "2";
    this->WriteData(requestDataToSend);
}


void Dialog::on_pushButtonCloseRoof_clicked()
{
    QByteArray requestDataToSend = "3";
    this->WriteData(requestDataToSend);
}


void Dialog::on_pushButtonOnPump_clicked()
{
    QByteArray requestDataToSend = "4";
    this->WriteData(requestDataToSend);
}


void Dialog::on_pushButtonOffPump_clicked()
{
    QByteArray requestDataToSend = "5";
    this->WriteData(requestDataToSend);
}

