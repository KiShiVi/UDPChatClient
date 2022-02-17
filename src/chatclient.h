#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QUdpSocket>

//!
//! \brief The ChatClient class Класс чат-клиента
//!
class ChatClient : public QWidget
{
    Q_OBJECT

public:
    //!
    //! \brief ChatClient Конструктор
    //! \param parent Родительский виджет
    //!
    ChatClient( QWidget *parent = nullptr, QHostAddress _targetHost = QHostAddress( "127.0.0.1" ),
               quint16 _targetPort = 229, quint16 _listeningPort = 228 );
    //!
    //! \brief ~ChatClient Деструктор
    //!
    virtual ~ChatClient();

public slots:
    //!
    //! \brief onReadyRead Слот, реагирующий на получение сообщения
    //!
    void onReadyRead();

private:
    //!
    //! \brief initGui Инициализация GUI
    //!
    void initGui();
    //!
    //! \brief sendDatagram Отправить сообщение
    //! \param message Текст сообщения
    //!
    void sendDatagram(const QString & message);

private slots:
    //!
    //! \brief onClickedApplyButton Слот, реагирующий на нажатие кнопки "Применить"
    //!
    void onClickedApplyButton();
    //!
    //! \brief onClickedSendButton Слот, реагирующий на нажатие кнопки "Отправить"
    //!
    void onClickedSendButton();
private:
//-------------------------------------------------------------------------------
    //!
    //! \brief socket UDP Сокет
    //!
    QUdpSocket * socket;
    //!
    //! \brief targetHost Адрес получателя
    //!
    QHostAddress targetHost;
    //!
    //! \brief targetPort Порт получателя
    //!
    quint16 targetPort;
    //!
    //! \brief listeningPort Прослушиваемый порт, для получения сообщения
    //!
    quint16 listeningPort;
//-------------------------------------------------------------------------------
    //!
    //! \brief targetHostTextBox Указатель на текст бокс адреса получателя
    //!
    QLineEdit * targetHostTextBox;
    //!
    //! \brief targetPortTextBox Указатель на текст бокс порта получателя
    //!
    QLineEdit * targetPortTextBox;
    //!
    //! \brief listeningPortTextBox Указатель на текст бокс порта прослушивания
    //!
    QLineEdit * listeningPortTextBox;
    //!
    //! \brief messageTextBox Указатель на текст бокс вводимого сообщения
    //!
    QLineEdit * messageTextBox;
    //!
    //! \brief messageBrowser Указатель на окно чата
    //!
    QTextEdit * messageBrowser;
};
