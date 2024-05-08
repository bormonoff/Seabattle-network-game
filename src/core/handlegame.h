#pragma once

#include "field.h"

#include <random>
#include <ctime>

#include <boost/asio.hpp>

namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace core {

class HandleGame{
public:
    HandleGame(Field& player);

    HandleGame(const HandleGame&) = delete;
    HandleGame& operator=(const HandleGame&) = delete;

    void StartGame(tcp::socket& socket, boost::system::error_code& ex, bool ServerTurn);

private:
    void EndGame();
    void PrintFields();
    bool IsGameEnd();
    bool CheckValid(std::string& msg);

    std::pair<int, int> DataFromString(const std::string& msg);

    Field playerField_;
    Field enemyField_;
    bool serverTurn_;
};

void PrintTwoFields(Field& player, Field& enemy);
std::optional<std::string> ReadExact(tcp::socket& socket, boost::system::error_code& ex,
                                     size_t count);
bool WriteExact(tcp::socket& socket, std::string& msg, boost::system::error_code& ex);
}  //namespace core
