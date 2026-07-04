#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "FairValueTracker.hpp"
#include <iostream>

TEST_CASE("Fair Value Tracker - successful test", "[FairValueTracker]") {
    GameEvent event{
        .playerId = "testPlayerId1",
        .gameId = "testGameId",
        .points = 13,
        .fieldGoalsMade = 4,
        .fieldGoalsAttempted = 11,
        .offensiveRebounds = 3,
        .defensiveRebounds = 8,
        .steals = 0,
        .assists = 2,
        .blockedShots = 0,
        .personalFouls = 3,
        .turnovers = 1,
        .freeThrowsMissed = 1
    };

    std::unordered_map<std::string, double> initialValues = { {"testPlayerId1", 10}, {"testPlayerId2", -0.5}, {"testPlayerId3", 20}};
    FairValueTracker tracker(0.8, initialValues);

    tracker.onGameEvent(event, 15);

    for (auto& [playerId, fairValue] : tracker.allFairValues()) {
        std::cout << "PlayerID: " << playerId << ", Fair Value: " << fairValue << std::endl;
    }

    REQUIRE_THAT(tracker.getFairValue("testPlayerId1"), Catch::Matchers::WithinAbs(14, 1e-9));
}