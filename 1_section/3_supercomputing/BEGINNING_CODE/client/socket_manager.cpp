#include "socket_manager.h"

#include <iostream>
#include <QTimer>
#include <QHostAddress>


SocketManager::SocketManager(int number_sockets,
                                                       QObject *parent) :
    QObject(parent),
    _total_sockets(number_sockets)
    {
        setup_sockets();
    }


void SocketManager::setup_sockets()
{
    for (int sock_num =0; sock_num < _total_sockets; sock_num++)
    {
        SocketPair socket_pair;
        _sockets.append(socket_pair);

        // input is from the push server
        socket_pair.input = new QTcpSocket();
        // output is to the pull server
        socket_pair.output = new QTcpSocket();
        socket_pair.output->connect(QHostAddress::LocalHost, 5001);

        QDataStream *in = new QDataStream();
        _data_streams.append(in);

        in->setDevice(socket_pair.input);

        connect(socket_pair.input, &QIODevice::readyRead, [this, sock_num, in](){

        });
    }

}

void SocketManager::start()
{
    std::cout << "Start" << std::endl << std::endl;
    _elasped_time.start();
    for (int i = 0; i < _total_sockets; i++)
    {
        QTcpSocket *socket = _sockets[i];
        socket->connectToHost(QHostAddress::LocalHost, 5000);
    }
}

void SocketManager::setup_socket_connections(QTcpSocket *socket, QDataStream *data_stream, int socket_number)
{
    connect(socket, &QIODevice::readyRead, [](){});
    connect(socket,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [](){});
}


void SocketManager::get_more_work(int socket_number)
{
    QTcpSocket *socket = _sockets[socket_number].input;
    socket->abort();
    socket->connectToHost(QHostAddress::LocalHost, 5000);
}

void SocketManager::do_work(QString work, SocketPair socket_pair, int socket_number)
{
    // NOTE: in real code, work calculations would happen before we compress the data again.

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << work;


    if (socket_pair.output->state() == QAbstractSocket::ConnectedState)
    {
        socket_pair.output->write(block);
    }

    std::cout << "Did work: " << work.toStdString() << " with socket # " << socket_number << std::endl;


    // int simulated_work_time = _random.bounded(500);
    // QTimer::singleShot(simulated_work_time, [this, sock_num](){get_more_work(sock_num);});
    QTimer::singleShot(simulated_work_time, [this, block, socket_number](){
        QTcpSocket *socket = _sockets[socket_number + _total_sockets];
    });

    if (work.toInt() == 100)
    {
        std::cout << "Time elasped: " << (float) _elasped_time.elapsed() / 1000 << " seconds" << std::endl;
        emit finished();
    }
}

void SocketManager::error_handler(QAbstractSocket::SocketError error, int socket_number)
{
    if (error == QAbstractSocket::ConnectionRefusedError)
    {
        QTimer::singleShot(1000, [this, socket_number](){reset_socket(socket_number);});
        if (_random.bounded(_total_sockets) == 0)
            std::cout << "No host found, is the server running?" << std::endl;
    }
}

void SocketManager::_setup_error_handling(QTcpSocket* socket)
{
    connect(socket,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [this, sock_num](QAbstractSocket::SocketError error){error_handler(error, sock_num);});
}
