#pragma once

#include <string>

struct GameEvent {
    std::string playerId;
    std::string gameId;
    std::time_t timestamp;
    int points, fieldGoalsMade, fieldGoalsAttempted, offensiveRebounds, defensiveRebounds, steals, assists, blockedShots, personalFouls, turnovers, freeThrowsMissed;
    bool teamWon;
};