#include "chatclient.h"

ChatClient::ChatClient( QWidget *parent, QHostAddress _targetHost,
                        quint16 _targetPort, quint16 _listeningPort )
    : QWidget( parent ),
      targetHost( _targetHost ),
      targetPort( _targetPort ),
      listeningPort( _listeningPort )
{
    socket = new QUdpSocket( this );
    socket->bind( QHostAddress::LocalHost, listeningPort );
    connect( socket, SIGNAL( readyRead() ), this, SLOT( onReadyRead() ) ); // MACRO UC_SAFE_CONNECT() :)
    initGui();
}

ChatClient::~ChatClient()
{
    delete socket;
}

void ChatClient::onReadyRead()
{
    while( socket->hasPendingDatagrams() ){
        QByteArray datagram;
        datagram.resize( socket->pendingDatagramSize() );
        socket->readDatagram( datagram.data(), datagram.size(), &targetHost, &targetPort );

        QTextCharFormat colorTool = messageBrowser->currentCharFormat();
        colorTool.setForeground( QBrush( Qt::red ) );
        messageBrowser->setCurrentCharFormat( colorTool );

        messageBrowser->append( targetHost.toString() + ": " + datagram.data() );

        colorTool.clearForeground();
        messageBrowser->setCurrentCharFormat( colorTool );

//        qDebug() << "IP: " << targetHost << "; Port: " << targetPort << "Message received: " << datagram.data();
//        qDebug() << targetPort;
//        qDebug() << listeningPort;
    }
}

void ChatClient::initGui()
{
    qDebug() << "Init GUI";

    this->setMinimumSize(500, 500);

    targetHostTextBox           = new QLineEdit( targetHost.toString() );
    targetPortTextBox           = new QLineEdit( QString::number( targetPort ) );
    listeningPortTextBox        = new QLineEdit( QString::number( listeningPort ) );

    QPushButton * applyButton = new QPushButton("Применить");

    connect( applyButton, SIGNAL( clicked() ), this, SLOT( onClickedApplyButton() ) ); // MACRO UC_SAFE_CONNECT() :)

    QHBoxLayout * settingsLayout = new QHBoxLayout();

    settingsLayout->addWidget( new QLabel( "Адрес\nполучателя:" ) );
    settingsLayout->addWidget( targetHostTextBox );
    settingsLayout->addWidget( new QLabel( "Порт\nполучателя:" ) );
    settingsLayout->addWidget( targetPortTextBox );
    settingsLayout->addWidget( new QLabel( "Порт\nпрослушивания:" ) );
    settingsLayout->addWidget( listeningPortTextBox );
    settingsLayout->addWidget( applyButton );

    messageTextBox                  = new QLineEdit();
    messageBrowser                  = new QTextEdit();
    QPushButton * sendButton        = new QPushButton("Отправить");

    connect( sendButton, SIGNAL( clicked() ), this, SLOT( onClickedSendButton() ) ); // MACRO UC_SAFE_CONNECT() :)

    QVBoxLayout * mainLayout = new QVBoxLayout();

    mainLayout->addLayout( settingsLayout );
    mainLayout->addWidget( messageBrowser );
    mainLayout->addWidget( messageTextBox );
    mainLayout->addWidget( sendButton );

    this->setLayout(mainLayout);
}

void ChatClient::sendDatagram( const QString & message )
{
    socket->writeDatagram( message.toUtf8(), targetHost, targetPort );

    QTextCharFormat colorTool = messageBrowser->currentCharFormat();
    colorTool.setForeground( QBrush( Qt::blue ) );
    messageBrowser->setCurrentCharFormat( colorTool );

    messageBrowser->append("You: " + message);

    colorTool.clearForeground();
    messageBrowser->setCurrentCharFormat( colorTool );


//    qDebug() << "IP: " << targetHost << "; Port: " << targetPort << "Message sent: " << message;
//    qDebug() << targetPort;
//    qDebug() << listeningPort;
}

void ChatClient::onClickedApplyButton()
{
    targetHost      = QHostAddress( targetHostTextBox->text() );
    targetPort      = targetPortTextBox->text().toUInt();
    listeningPort   = listeningPortTextBox->text().toUInt();
    qDebug() << targetPort;
    qDebug() << listeningPort;

    socket->close();

    socket->bind( QHostAddress::LocalHost, listeningPort );

//    socket->abort();
//    socket->bind( QHostAddress::LocalHost, listeningPort );
}

void ChatClient::onClickedSendButton()
{
    sendDatagram( messageTextBox->text() );
}

