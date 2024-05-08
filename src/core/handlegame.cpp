#include "handlegame.h"

namespace core {

void PrintTwoFields(Field& player, Field& enemy) {
    char sign{'A'};
    std::string tab = "  ";
    std::string gap = "     ";

    std::cout << tab;
    player.PrintHeadLine(std::cout);
    std::cout << gap;
    enemy.PrintHeadLine(std::cout);
    std::cout << std::endl;

    for (int i = 0; i < player.fieldSize_; ++i) {
        std::cout << tab;
        player.PrintFieldLine(std::cout, i);
        std::cout << gap;
        enemy.PrintFieldLine(std::cout, i);
        std::cout << std::endl;
    }

    std::cout << tab;
    player.PrintHeadLine(std::cout);
    std::cout << gap;
    enemy.PrintHeadLine(std::cout);
    std::cout << std::endl;
}

std::optional<std::string> ReadExact(tcp::socket& socket,
                                            boost::system::error_code& ex,
                                            size_t count) {
    std::vector<char> buff(count);
    net:read(socket, net::buffer(buff), net::transfer_exactly(count), ex);
    if (ex) {
        std::cout << "reading error" << std::endl;
        return std::nullopt;
    }
    return {{buff.data(), count}};
}

bool WriteExact(tcp::socket& socket, std::string& msg,
                       boost::system::error_code& ex) {
    net::write(socket, net::buffer(msg), net::transfer_exactly(msg.size()), ex);
    return !ex;
}


HandleGame::HandleGame(Field& player)
    :playerField_{player},
    serverTurn_{1} {
}

void HandleGame::StartGame(tcp::socket& socket, boost::system::error_code& ex,
                           bool serverTurn_) {
while (!IsGameEnd())
    if (serverTurn_) {
        PrintFields();
        std::string coord {};
        std::cout << "Enter coordinate: " << std::endl;
        std::cin>>coord;

        if (CheckValid(coord)) {
            std::cout << "Wrong data format, Enter char & number." << std::endl;
            continue;
        }
        if (!WriteExact(socket, coord, ex)) {
            std::cout << "Connection was corrupted" << std::endl;
        }
        std::string result {*(ReadExact(socket, ex, 4))};
        std::pair<size_t,size_t> temp = DataFromString(coord);
        std::cout << "The result is: " << result << std::endl;
        enemyField_.MarkCell(temp.first, temp.second, result);

        if (result == "MISS") {
            serverTurn_ = 0;
        } else{
            enemyField_.health_--;
        }
        if (IsGameEnd()) {
            break;
        }
    } else {
        PrintFields();
        std::cout << "Wait for your turn" << std::endl;
        std::string temp {*(ReadExact(socket, ex, 2))};
        std::cout << "The enemy shooted as: " << temp << std::endl;
        std::pair shootCoord {DataFromString(temp)};
        temp = playerField_.Shoot(shootCoord.first, shootCoord.second);
        if (temp == "MISS") {
            serverTurn_ = 1;
        }
        if (!WriteExact(socket, temp, ex)) {
            std::cout << "Connection was corrupted" << std::endl;
        }
        if (IsGameEnd()) {
            break;
        }
    }
    EndGame();
}

void HandleGame::PrintFields() {
    PrintTwoFields(playerField_, enemyField_);
}

bool HandleGame::IsGameEnd() {
    if (playerField_.health_ == 0 ||
        enemyField_.health_ == 0) {
        return true;
    }
    return false;
}

void HandleGame::EndGame() {
    std::cout << "The game is ended" << std::endl;
    if (playerField_.health_ == 0) {
        std::cout << "You fail!" << std::endl;
    } else {
        std::cout << "You win!" << std::endl;
    }
    PrintFields();
}

bool HandleGame::CheckValid(std::string& msg) {
    if (msg.size() != 2) {
        std::cout << "Type correct field!" << std::endl;
        return true;
    }
    int x = (static_cast<size_t>(msg.at(0)) - 'A');
    int y = (static_cast<size_t>(msg.at(1)) - '0');
    if (x > Field::fieldSize_ || x < 0 ) {
        return true;
    }
    if (y > Field::fieldSize_ || y < 0) {
        return true;
    }
    return false;
}

std::pair<int, int> HandleGame::DataFromString(const std::string& msg) {
    size_t x = (static_cast<size_t>(msg.at(0)) - 65);
    size_t y = (static_cast<size_t>(msg.at(1)) - 49);
    return {x,y};
}
}  // namespace core
