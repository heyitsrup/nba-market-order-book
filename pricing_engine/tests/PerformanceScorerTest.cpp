#include <catch2/catch_test_macros.hpp>
#include "PerformanceScorer.hpp"
#include "DataLoader.hpp"

TEST_CASE("Performance scorer success run", "[PerformanceScorer]") {
    std::string filename = "/Users/rup/Git/nba-market-order-book/pricing_engine/data/boxscores.csv";
    auto events = DataLoader::loadEvents(filename);
    GameEvent event = events[0];
    REQUIRE(PerformanceScorer::score(event) == 10.1);
}