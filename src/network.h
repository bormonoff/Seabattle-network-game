#pragma once

#include "handlegame.h"
#include <fstream>

namespace net = boost::asio;
using tcp = net::ip::tcp;

static void StartServer(HandleGame& game, const int& port) {
    net::io_context io; 
    tcp::acceptor acceptor(io,tcp::endpoint(net::ip::address::from_string("0.0.0.0"), 
                                            port));
    std::cout<<"Waiting for connection!"<<std::endl;

    boost::system::error_code ex;
    tcp::socket socket{io};
    acceptor.accept(socket, ex);
    if (!ex) {
        std::cout<<"Connected sucesfully!"<<std::endl;
    } else {
        std::cout<<"Connection failed!"<<std::endl;
    }
    game.StartGame(socket, ex, true);
}

static void StartClient(HandleGame& game, const int& port, const std::string& IP) {
    net::io_context io; 
    std::cout<<"Waiting for connection!"<<std::endl;

    boost::system::error_code ex;
    tcp::socket socket{io};
    socket.connect(tcp::endpoint(net::ip::address::from_string(IP), port),ex);
    if (!ex) {
        std::cout<<"Connected sucesfully!"<<std::endl;
    } else {
        std::cout<<"Connection failed!"<<std::endl;
    }
    game.StartGame(socket, ex, false);
}
