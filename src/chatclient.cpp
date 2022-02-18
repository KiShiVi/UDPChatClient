#include "chatclient.h"

ChatClient::ChatClient( QWidget * parent, const QHostAddress & _targetHost,
                        const quint16 _targetPort, const quint16 _listeningPort )
    : QWidget( parent ),
      targetHost( _targetHost ),
      targetPort( _targetPort ),
      listeningPort( _listeningPort )
{
    socket = new QUdpSocket( this );
    socket->bind( QHostAddress::Any, listeningPort );
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

        if ( QString( datagram.data() ).isEmpty() )
            return;

        QTextCharFormat colorTool = messageBrowser->currentCharFormat();
        colorTool.setForeground( QBrush( Qt::red ) );
        messageBrowser->setCurrentCharFormat( colorTool );

        messageBrowser->append( QHostAddress( targetHost.toIPv4Address() ).toString() + ": " + datagram.data() );

        messageBrowser->verticalScrollBar()->setValue( messageBrowser->verticalScrollBar()->maximum() );

        colorTool.clearForeground();
        messageBrowser->setCurrentCharFormat( colorTool );
    }
}

void ChatClient::initGui()
{
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

    messageBrowser->setReadOnly( true );

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
    if ( message.isEmpty() )
        return;

    socket->writeDatagram( message.toUtf8(), targetHost, targetPort );

    QTextCharFormat colorTool = messageBrowser->currentCharFormat();
    colorTool.setForeground( QBrush( Qt::blue ) );
    messageBrowser->setCurrentCharFormat( colorTool );

    messageBrowser->append("You: " + message);

    colorTool.clearForeground();
    messageBrowser->setCurrentCharFormat( colorTool );

    messageBrowser->verticalScrollBar()->setValue( messageBrowser->verticalScrollBar()->maximum() );
}

void ChatClient::keyPressEvent( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
        onClickedSendButton();
}

void ChatClient::onClickedApplyButton()
{
    targetHost      = QHostAddress( targetHostTextBox->text() );
    targetPort      = targetPortTextBox->text().toUInt();
    listeningPort   = listeningPortTextBox->text().toUInt();

    socket->close();

    socket->bind( QHostAddress::Any, listeningPort );
}

void ChatClient::onClickedSendButton()
{
    sendDatagram( messageTextBox->text() );
    messageTextBox->clear();
}

