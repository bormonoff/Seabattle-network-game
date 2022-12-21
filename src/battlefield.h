#pragma once

#include <string>
#include <iostream>
#include <array>

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
        list[x*2] = Sign(m_field[ x + x*y ]);
        if(x*2+1<m_fieldSize*2-1){
        list[x*2+1] = ' ';}
        }
        out.write(list.begin(), list.size());
    }

    void PrintHeadLine(std::ostream& out){
        out<<"    ";
        out<<"A B C D E F G H";
    }

    char Sign(State state){
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
   private:
   std::array<State, m_fieldSize * m_fieldSize -1> m_field;
};

