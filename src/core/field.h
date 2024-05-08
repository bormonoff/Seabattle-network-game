#pragma once

#include <array>
#include <iostream>
#include <optional>
#include <random>

namespace core {

class Field {
public:
    enum class State {
        NOTSTATE,
        EMPTY,
        LIVE,
        KILLED,
        HIT
    };

    Field(State state = State::NOTSTATE);

    Field(const Field&) = default;
    Field& operator=(const Field&) = default;

    void PrintFieldLine(std::ostream& out, size_t y);
    void PrintHeadLine(std::ostream& out);
    std::string Shoot(size_t x, size_t y);
    void MarkCell(size_t x, size_t y, const std::string& result);

    static Field GenerateField(std::mt19937&& engine);
    static const short fieldSize_ = 8;

    std::array<State, 64> field_;
    int health_;

private:
    enum class Result {
        MISS = 0,
        HIT  = 1,
        KILL = 2
    };

    static std::optional<Field> TryGenerateField(std::mt19937&& random_engine);

    void MarkAfterDeath (size_t x, size_t y);
    const char Sign(State state);
    bool IsDead (size_t x, size_t y);
    bool CheckDeadInDirection(size_t x, size_t y, int dx, int dy);
    State& Get (size_t x, size_t y);
};
}  // namespace core
