#include "PerformanceScorer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Performance scorer - no DataLoader success run", "[PerformanceScorer]") {

    GameEvent event{.playerId = "testPlayerId",
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
                    .freeThrowsMissed = 1};
    // Don't compare with == use tolerance
    REQUIRE_THAT(PerformanceScorer::score(event), Catch::Matchers::WithinAbs(10.2, 1e-9));
}