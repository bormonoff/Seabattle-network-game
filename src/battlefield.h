#pragma once

#include <string>
#include <iostream>
#include <array>
#include <set>
#include <optional>
#include <vector>
#include <random>

class Field
{
    enum class State{
        NOTSTATE,
        EMPTY,
        LIVE,
        KILLED,
    }; 

    public:
    static const short m_fieldSize = 8;

    Field(State state = State::NOTSTATE){
        m_field.fill(state);
    }
    
        void PrintFieldLine(std::ostream& out, size_t y){
        std::array<char, m_fieldSize * 2-1> list;
        for (int x = 0; x < m_fieldSize; x++){
        list[x*2] = Sign(m_field[x + y * m_fieldSize]);
        if(x*2+1<m_fieldSize*2-1){
        list[x*2+1] = ' ';}
        }
        out.write(list.begin(), list.size());
    }

    void PrintHeadLine(std::ostream& out){
        out<<"    ";
        out<<"A B C D E F G H";
    }

      template<typename T>
    static Field GenerateField(T&& engine){
        std::optional<Field> player;
        do{
            player = TryGenerateField(engine);
        }while(!player);
        return *player;
    }

private:
    //Generate player field using sizes vector
    template<typename T>
    static std::optional<Field> TryGenerateField(T&& random_engine){
        std::set<std::pair<size_t, size_t>> availiableElements;
        std::vector<int> sizes{4,3,3,2,2,1,1};
        Field temp {State::EMPTY};

        for (size_t x = 0; x < m_fieldSize; x++){
            for (size_t y = 0; y < m_fieldSize; y++){
                availiableElements.insert({x,y});
            }
        }
        
        const auto markAsUsed = [&](size_t x, size_t y) {
            for (int dx = -1;  dx <= 1; dx++){
                for (int dy = -1; dy <= 1; dy++){
                    availiableElements.erase({x + dx, y + dy});
                }                
            }
        };

        const auto dirTo = [](size_t dir) -> std::pair<int, int> {
        int dx = dir == 1 ? 1 : (dir == 3 ) ? -1 : 0;
        int dy = dir == 0 ? 1 : dir == 2 ? -1 : 0;
        return {dx, dy};
        };

        const auto CheckShip = [&](size_t x, size_t y, size_t dir, int lengh){
        std::pair[dx, dy] = dirTo(dir);
        for (int i = 0; i < lengh; i++)
        {
            size_t cx = x + dx*i;
            size_t cy = y + dy*i;

            if(cx >= m_fieldSize || cy >= m_fieldSize){
                return false;
            }
            
            if(availiableElements.count({cx, cy})==0){
                return false;
            }
        }
            return true;        
        };

        const auto MarkShip = [&](size_t x, size_t y, size_t dir, int lengh){
        std::pair[dx, dy] = dirTo(dir);
        for (int i = 0; i < lengh; i++)
        {
            size_t cx = x + dx*i;
            size_t cy = y + dy*i;

            temp.Get(cx, cy) = State::LIVE;
            markAsUsed(cx, cy);
        }};

        using Random = std::uniform_int_distribution<int>;
        using Range = Random::param_type;

        for(int shipSize : sizes){
        std::pair<int, int> pos;
        Random a;
        size_t dir;

        size_t attempts = 100;
        do{
        attempts--;
            if(attempts<=0){
                return std::nullopt;
            }
        size_t pos_index = a(random_engine, Range(0,availiableElements.size() - 1));
        dir = a(random_engine, Range(0, 3));
        pos = *std::next(availiableElements.begin(), pos_index);
        } while (!CheckShip(pos.first, pos.second, dir, shipSize));
        MarkShip(pos.first, pos.second, dir, shipSize);
        }
        return temp;
    }

    const char Sign(State state){
        switch(state){
        case State::NOTSTATE:
            return '?';
        case State::EMPTY:
            return '.';
        case State::LIVE:
            return 'O';
        case State::KILLED:
            return 'X';
        }}
 
   State& Get (size_t x, size_t y){
        return m_field[x+ y*m_fieldSize];
    }

private:
   std::array<State, m_fieldSize * m_fieldSize> m_field;
};

