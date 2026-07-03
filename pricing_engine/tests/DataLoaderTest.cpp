#include <catch2/catch_test_macros.hpp>
#include "DataLoader.hpp"

TEST_CASE("Load Events", "[pricing_engine: loadEvents]") {
    std::string filename = "/Users/rup/Git/nba-market-order-book/pricing_engine/data/boxscores.csv";
    auto events = DataLoader::loadEvents(filename);
    REQUIRE(events.size() > 0);
}