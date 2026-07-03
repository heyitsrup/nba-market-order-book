#include <catch2/catch_test_macros.hpp>
#include "DataLoader.hpp"
#include <fstream>
#include <cstdio>

TEST_CASE("Load Events throws on missing file", "[DataLoader]") {
    std::string filename = "/path/does/not/exist.csv";
    REQUIRE_THROWS_AS(DataLoader::loadEvents(filename), DataLoaderFileException);
}

TEST_CASE("Load Events - malformed row is skipped", "[DataLoader]") {
    std::string filename = "test_malformed.csv";
    {
        std::ofstream out(filename);
        out << "only,three,fields\n";
    }

    std::vector<GameEvent> events;
    REQUIRE_NOTHROW(events = DataLoader::loadEvents(filename));
    REQUIRE(events.empty());
    std::remove(filename.c_str());
}

TEST_CASE("Load Events successful run", "[pricing_engine: loadEvents]") {
    std::string filename = "/Users/rup/Git/nba-market-order-book/pricing_engine/data/boxscores.csv";
    auto events = DataLoader::loadEvents(filename);
    REQUIRE(events.size() > 0);
}