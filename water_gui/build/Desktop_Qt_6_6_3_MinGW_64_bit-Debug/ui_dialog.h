/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *pushButtonAuto;
    QPushButton *pushButtonStop;
    QLCDNumber *lcdTemperature;
    QLabel *label;
    QLabel *label_2;
    QLCDNumber *lcdHumidity;
    QLabel *soil_moisture;
    QLCDNumber *lcdSoilmoisture;
    QLabel *time;
    QTextEdit *timePump;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QFrame *frame;
    QFrame *frame_2;
    QFrame *frame_3;
    QPushButton *pushButtonCloseRoof;
    QPushButton *pushButtonOpenRoof;
    QPushButton *pushButtonOnPump;
    QPushButton *pushButtonOffPump;
    QComboBox *comboBoxPort;
    QComboBox *comboBoxBaudRate;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonDisconnect;
    QLabel *label_9;
    QTextEdit *textEditGetData;
    QPushButton *pushButtonExit;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(800, 600);
        QFont font;
        font.setPointSize(10);
        Dialog->setFont(font);
        pushButtonAuto = new QPushButton(Dialog);
        pushButtonAuto->setObjectName("pushButtonAuto");
        pushButtonAuto->setGeometry(QRect(450, 90, 131, 71));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI Black")});
        font1.setPointSize(17);
        font1.setBold(true);
        font1.setItalic(false);
        pushButtonAuto->setFont(font1);
        pushButtonAuto->setStyleSheet(QString::fromUtf8("\n"
"color: rgb(255, 0, 0);"));
        pushButtonStop = new QPushButton(Dialog);
        pushButtonStop->setObjectName("pushButtonStop");
        pushButtonStop->setGeometry(QRect(600, 90, 131, 71));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Segoe UI Black")});
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setItalic(false);
        pushButtonStop->setFont(font2);
        pushButtonStop->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        lcdTemperature = new QLCDNumber(Dialog);
        lcdTemperature->setObjectName("lcdTemperature");
        lcdTemperature->setGeometry(QRect(600, 290, 131, 41));
        label = new QLabel(Dialog);
        label->setObjectName("label");
        label->setGeometry(QRect(450, 290, 131, 41));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setItalic(true);
        label->setFont(font3);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(Dialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(450, 350, 131, 41));
        label_2->setFont(font3);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        label_2->setAlignment(Qt::AlignCenter);
        lcdHumidity = new QLCDNumber(Dialog);
        lcdHumidity->setObjectName("lcdHumidity");
        lcdHumidity->setGeometry(QRect(600, 350, 131, 41));
        soil_moisture = new QLabel(Dialog);
        soil_moisture->setObjectName("soil_moisture");
        soil_moisture->setGeometry(QRect(450, 410, 131, 41));
        soil_moisture->setFont(font3);
        soil_moisture->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        soil_moisture->setAlignment(Qt::AlignCenter);
        lcdSoilmoisture = new QLCDNumber(Dialog);
        lcdSoilmoisture->setObjectName("lcdSoilmoisture");
        lcdSoilmoisture->setGeometry(QRect(600, 410, 131, 41));
        time = new QLabel(Dialog);
        time->setObjectName("time");
        time->setGeometry(QRect(450, 480, 131, 41));
        time->setFont(font3);
        time->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        time->setAlignment(Qt::AlignCenter);
        timePump = new QTextEdit(Dialog);
        timePump->setObjectName("timePump");
        timePump->setGeometry(QRect(600, 480, 131, 41));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 10, 151, 21));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 50, 151, 21));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 90, 151, 21));
        frame = new QFrame(Dialog);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(410, 260, 361, 291));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 127);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(30, 70, 361, 291));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-color: rgb(85, 255, 127);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_3 = new QFrame(Dialog);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(20, 350, 351, 201));
        frame_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-color: rgb(0, 0, 0);\n"
"background-color: rgb(85, 255, 127);"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        pushButtonCloseRoof = new QPushButton(frame_3);
        pushButtonCloseRoof->setObjectName("pushButtonCloseRoof");
        pushButtonCloseRoof->setGeometry(QRect(200, 20, 131, 71));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Segoe UI Black")});
        font4.setPointSize(10);
        font4.setBold(true);
        font4.setItalic(false);
        pushButtonCloseRoof->setFont(font4);
        pushButtonCloseRoof->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(255, 0, 0);"));
        pushButtonOpenRoof = new QPushButton(frame_3);
        pushButtonOpenRoof->setObjectName("pushButtonOpenRoof");
        pushButtonOpenRoof->setGeometry(QRect(30, 20, 131, 71));
        pushButtonOpenRoof->setFont(font4);
        pushButtonOpenRoof->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(255, 0, 0);"));
        pushButtonOnPump = new QPushButton(frame_3);
        pushButtonOnPump->setObjectName("pushButtonOnPump");
        pushButtonOnPump->setGeometry(QRect(30, 110, 131, 71));
        pushButtonOnPump->setFont(font4);
        pushButtonOnPump->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(255, 0, 0);"));
        pushButtonOffPump = new QPushButton(frame_3);
        pushButtonOffPump->setObjectName("pushButtonOffPump");
        pushButtonOffPump->setGeometry(QRect(200, 110, 131, 71));
        pushButtonOffPump->setFont(font4);
        pushButtonOffPump->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(255, 0, 0);"));
        comboBoxPort = new QComboBox(Dialog);
        comboBoxPort->setObjectName("comboBoxPort");
        comboBoxPort->setGeometry(QRect(110, 50, 91, 31));
        comboBoxBaudRate = new QComboBox(Dialog);
        comboBoxBaudRate->setObjectName("comboBoxBaudRate");
        comboBoxBaudRate->setGeometry(QRect(110, 90, 91, 31));
        label_6 = new QLabel(Dialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(420, 220, 151, 31));
        label_7 = new QLabel(Dialog);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 310, 171, 31));
        label_8 = new QLabel(Dialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(450, 50, 171, 31));
        pushButtonConnect = new QPushButton(Dialog);
        pushButtonConnect->setObjectName("pushButtonConnect");
        pushButtonConnect->setGeometry(QRect(220, 50, 131, 31));
        pushButtonConnect->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
""));
        pushButtonDisconnect = new QPushButton(Dialog);
        pushButtonDisconnect->setObjectName("pushButtonDisconnect");
        pushButtonDisconnect->setGeometry(QRect(220, 90, 131, 31));
        pushButtonDisconnect->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
""));
        label_9 = new QLabel(Dialog);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 130, 171, 31));
        textEditGetData = new QTextEdit(Dialog);
        textEditGetData->setObjectName("textEditGetData");
        textEditGetData->setGeometry(QRect(30, 170, 331, 101));
        pushButtonExit = new QPushButton(Dialog);
        pushButtonExit->setObjectName("pushButtonExit");
        pushButtonExit->setGeometry(QRect(732, 10, 61, 29));
        pushButtonExit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        frame->raise();
        pushButtonAuto->raise();
        pushButtonStop->raise();
        soil_moisture->raise();
        label->raise();
        lcdSoilmoisture->raise();
        label_2->raise();
        lcdHumidity->raise();
        time->raise();
        lcdTemperature->raise();
        timePump->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        frame_3->raise();
        comboBoxPort->raise();
        comboBoxBaudRate->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        pushButtonConnect->raise();
        pushButtonDisconnect->raise();
        label_9->raise();
        textEditGetData->raise();
        pushButtonExit->raise();

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "WaterController", nullptr));
        pushButtonAuto->setText(QCoreApplication::translate("Dialog", "Auto", nullptr));
        pushButtonStop->setText(QCoreApplication::translate("Dialog", "Stop", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "TEMPERATURE", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "HUMIDITY", nullptr));
        soil_moisture->setText(QCoreApplication::translate("Dialog", "SOIL MOISTURE", nullptr));
        time->setText(QCoreApplication::translate("Dialog", "LAST PUMP", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "K\341\272\277t n\341\273\221i Serial Port: ", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "Port:", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "Baud Rate: ", nullptr));
        pushButtonCloseRoof->setText(QCoreApplication::translate("Dialog", "Close Roof", nullptr));
        pushButtonOpenRoof->setText(QCoreApplication::translate("Dialog", "Open Roof", nullptr));
        pushButtonOnPump->setText(QCoreApplication::translate("Dialog", "On Pump", nullptr));
        pushButtonOffPump->setText(QCoreApplication::translate("Dialog", "Off Pump", nullptr));
        label_6->setText(QCoreApplication::translate("Dialog", "Hi\341\273\203n th\341\273\213 th\303\264ng s\341\273\221:", nullptr));
        label_7->setText(QCoreApplication::translate("Dialog", "\304\220i\341\273\201u khi\341\273\203n th\341\273\247 c\303\264ng: ", nullptr));
        label_8->setText(QCoreApplication::translate("Dialog", "\304\220i\341\273\201u khi\341\273\203n t\341\273\261 \304\221\341\273\231ng: ", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("Dialog", "CONNECT", nullptr));
        pushButtonDisconnect->setText(QCoreApplication::translate("Dialog", "DISCONNECT", nullptr));
        label_9->setText(QCoreApplication::translate("Dialog", "Hi\341\273\203n th\341\273\213:", nullptr));
        pushButtonExit->setText(QCoreApplication::translate("Dialog", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
