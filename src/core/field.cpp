#include "field.h"

#include <string>
#include <set>
#include <vector>

namespace core {

Field::Field(Field::State state) {
    health_ = 4 + 3 + 3 + 2 + 2 + 2 + 1 + 1;
    field_.fill(state);
}

void Field::PrintFieldLine(std::ostream& out, size_t y) {
    std::array<char, fieldSize_ * 2-1> list;
    for (int x = 0; x < fieldSize_; ++x) {
        list[x * 2] = Sign(field_[x + y * fieldSize_]);
        if (x * 2 + 1 < fieldSize_ * 2 - 1) {
            list[x * 2 + 1] = ' ';
        }
    }
    out << 1 + y ;
    out << " ";
    out.write(list.begin(), list.size());
    out << " ";
    out << 1 + y;
}

void Field::PrintHeadLine(std::ostream& out) {
    out << "  ";
    out << "A B C D E F G H";
    out << "  ";
}

Field Field::GenerateField(std::mt19937&& engine) {
    std::optional<Field> player;
    do {
        player = TryGenerateField(std::move(engine));
    } while(!player);
    return *player;
}

std::optional<Field> Field::TryGenerateField(std::mt19937&& random_engine) {
    std::set<std::pair<size_t, size_t>> availiableElements;
    std::vector<int> sizes{4,3,3,2,2,2,1,1};
    Field temp {State::EMPTY};
    for (size_t x = 0; x < fieldSize_; ++x) {
        for (size_t y = 0; y < fieldSize_; ++y) {
            availiableElements.insert({x,y});
        }
    }

    const auto markAsUsed = [&](size_t x, size_t y) {
        for (int dx = -1;  dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                availiableElements.erase({x + dx, y + dy});
            }
        }
    };

    const auto dirTo = [](size_t dir) -> std::pair<int, int> {
        int dx = dir == 1 ? 1 : (dir == 3 ) ? -1 : 0;
        int dy = dir == 0 ? 1 : dir == 2 ? -1 : 0;
        return {dx, dy};
    };

    const auto CheckShip = [&](size_t x, size_t y, size_t dir, int lengh) {
    auto [dx, dy] = dirTo(dir);
    for (int i = 0; i < lengh; ++i) {
        size_t cx = x + dx * i;
        size_t cy = y + dy * i;
        if (cx >= fieldSize_ || cy >= fieldSize_) {
            return false;
        }
        if (availiableElements.count({cx, cy}) == 0) {
            return false;
        }
    }
        return true;
    };

    const auto MarkShip = [&](size_t x, size_t y, size_t dir, int lengh) {
    auto [dx, dy] = dirTo(dir);
    for (int i = 0; i < lengh; ++i) {
        size_t cx = x + dx * i;
        size_t cy = y + dy * i;
        temp.Get(cx, cy) = State::LIVE;
        markAsUsed(cx, cy);
    }};

    using Random = std::uniform_int_distribution<int>;
    using Range = Random::param_type;

    for (int shipSize : sizes) {
        std::pair<int, int> pos;
        Random a;
        size_t dir;
        size_t attempts = 100;
        do {
            attempts--;
                if (attempts <= 0) {
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

const char Field::Sign(State state) {
    switch(state){
    case State::NOTSTATE:
        return '?';
    case State::EMPTY:
        return '.';
    case State::LIVE:
        return 'O';
    case State::KILLED:
        return 'X';
    case State::HIT:
        return '*';
    default:
        return '-';
    }}

Field::State& Field::Get (size_t x, size_t y) {
    return field_[x+ y*fieldSize_];
}

bool Field::IsDead (size_t x, size_t y) {
    return CheckDeadInDirection(x,y,1,0) && CheckDeadInDirection(x,y,0,1)
        && CheckDeadInDirection(x,y,-1,0) && CheckDeadInDirection(x,y,0,-1);
}

bool Field::CheckDeadInDirection(size_t x, size_t y, int dx, int dy) {
    for (;x <= fieldSize_, y <= fieldSize_; x += dx, y+= dy){
        if(Get(x,y) == State::LIVE){return 0;}
        if(Get(x,y) == State::EMPTY){return 1;}
    }
    return true;
}

void Field::MarkAfterDeath (size_t x, size_t y) {
    auto MarkEmpty = [&](size_t x, size_t y) {
        if (Get(x,y) != State::NOTSTATE) {
            return;
        }
        if (x>=fieldSize_ || y >= fieldSize_) {
            return;
        }
        Get(x,y) = State::EMPTY;
    };

    auto MarkCycle = [&](size_t x, size_t y, int dx, int dy) {
        for (;x<fieldSize_ && y < fieldSize_; x += dx, y += dy) {
            MarkEmpty(x-dy, y-dx);
            MarkEmpty(x+dy, y+dx);
            MarkEmpty(x,y);
            if (Get(x,y) != State::KILLED) {
                return;
            }
        }
    };

    MarkCycle(x,y,0,1);
    MarkCycle(x,y,0,-1);
    MarkCycle(x,y,-1,0);
    MarkCycle(x,y,-1,0);
}

std::string Field::Shoot(size_t x, size_t y) {
    if (Get(x,y) != State::LIVE) {
        return std::string{"MISS"};
    } else {
        Get(x,y) = State::KILLED;
        health_--;
        if (IsDead(x,y)) {
            return std::string{"KILL"};
        } else {
            return std::string{"HIT "};
        };
    }
}

void Field::MarkCell(size_t x, size_t y, const std::string& result) {
    if( result == "MISS") {
        Get(x,y) = State::EMPTY;
    } else {
        Get(x,y) = State::KILLED;
        if(result == "KILL"){
            MarkAfterDeath(x,y);
        }
    }
};
}  // namespace core
