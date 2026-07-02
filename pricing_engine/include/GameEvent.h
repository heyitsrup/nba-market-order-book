#pragma once

#include <string>

struct GameEvent {
    std::string playerId;
    std::string gameId;
    std::time_t timestamp;
    int pts, fgm, fga, ftm, fta, orb, drb, stl, ast, blk, pf, tov;
    bool teamWon;
};