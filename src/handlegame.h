#pragma once

#include "battlefield.h"
#include <boost/asio.hpp>
#include <random>
#include <ctime>

namespace net = boost::asio;
using tcp = net::ip::tcp;

static void PrintTwoFields(Field& player, Field& enemy){
    char sign{'A'};
    std::string tab = "  ";
    std::string gap = "     ";

    std::cout<<tab;
    player.PrintHeadLine(std::cout);
    std::cout<<gap;
    enemy.PrintHeadLine(std::cout);
    std::cout<<std::endl;
    for (int i = 0; i < player.m_fieldSize; i++)
    {   
        std::cout<<tab;
        player.PrintFieldLine(std::cout, i);
        std::cout<<gap;
        enemy.PrintFieldLine(std::cout, i);
        std::cout<<std::endl;
    }
    std::cout<<tab;
    player.PrintHeadLine(std::cout);
    std::cout<<gap;
    enemy.PrintHeadLine(std::cout);
    std::cout<<std::endl;
}

template <size_t count>
static std::optional<std::string> ReadExact(tcp::socket& socket, boost::system::error_code& ex){
    std::array<char, count> buff;

    net:read(socket, net::buffer(buff), net::transfer_exactly(count), ex);

    if(ex){
        std::cout<<"reading error"<<std::endl;
        return std::nullopt;
    }

    return {{buff.data(), count}};
} 

static bool WriteExact(tcp::socket& socket, std::string& msg, boost::system::error_code& ex){
    net::write(socket, net::buffer(msg), net::transfer_exactly(msg.size()), ex);

    return !ex;
}

class HandleGame{
public:

    void StartGame(tcp::socket& socket, boost::system::error_code& ex, bool ServerTurn){
    while(!IsGameEnd())
        if(ServerTurn){
            PrintFields();

            std::string coord {};
            std::cout<<"Enter coordinate: "<<std::endl;
            std::cin>>coord;

            if(CheckValid(coord)){
                std::cout<<"Wrong data format, Enter char & number."<<std::endl;
                continue;   
            }
        
            if(!WriteExact(socket, coord, ex)){
                std::cout<<"Connection was corrupted"<<std::endl;
            }

            std::string result {*(ReadExact<4>(socket, ex))};
            std::pair<size_t,size_t> temp = DataFromString(coord);
            std::cout<<"The result is: "<<result<<std::endl;
            m_enemyField.MarkCell(temp.first, temp.second, result);

            if(result == "MISS"){
                ServerTurn = 0;
            }else{
                m_enemyField.health--;
            }

            if(IsGameEnd()){
                break;
            }

        }else{
            PrintFields();

            std::cout<<"Wait for your turn"<<std::endl;

            std::string temp {*(ReadExact<2>(socket, ex))};
            std::cout<<"The enemy shooted as: "<<temp<<std::endl;
            std::pair shootCoord {DataFromString(temp)};
            temp = m_myField.Shoot(shootCoord.first, shootCoord.second);

            if(temp == "MISS"){
                ServerTurn = 1;
            }

            if(!WriteExact(socket, temp, ex)){
                std::cout<<"Connection was corrupted"<<std::endl;
            }
            
            if(IsGameEnd()){
                break;
            }
        }
        EndGame();
    }

    HandleGame(Field& player)
        :m_myField{player}, ServerTurn{1}{
    }

    void PrintFields(){
        PrintTwoFields(m_myField, m_enemyField);
    }
    
    bool IsGameEnd(){
        if(m_myField.health == 0 || 
           m_enemyField.health == 0){
            return true;
        }
        return false;
    }

    void EndGame(){
        std::cout<<"The game is ended"<<std::endl;  
        if(m_myField.health == 0){
            std::cout<<"You fail!"<<std::endl;
        }else{
            std::cout<<"You win!"<<std::endl;
        }
        PrintFields();
    }

public:

    bool CheckValid(std::string& msg){
        if(msg.size() != 2){
            std::cout<<"Type correct field!"<<std::endl;
            return true;
        }
       
        int x = (static_cast<size_t>(msg.at(0)) - 'A');
        int y = (static_cast<size_t>(msg.at(1)) - '0');

        if(x > Field::m_fieldSize || x < 0 ){
            return true;
        }
        if( y > Field::m_fieldSize || y < 0){
            return true;
        }
        return false;
    }

    static std::pair<int, int> DataFromString(const std::string& msg){
        
        size_t x = (static_cast<size_t>(msg.at(0)) - 65);
        size_t y = (static_cast<size_t>(msg.at(1)) - 49);
        
        return {x,y};
    }

private:
    Field m_myField;
    Field m_enemyField;
    bool ServerTurn;
};
