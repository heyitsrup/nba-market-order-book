#include "DataLoader.hpp"
#include "GameEvent.hpp"
#include "PerformanceScorer.hpp"
#include "FairValueTracker.hpp"
#include <string>
#include <iostream>

int main()
{
    std::string filename = "/Users/rup/Git/nba-market-order-book/pricing_engine/data/boxscores.csv";
    auto events = DataLoader::loadEvents(filename);
    FairValueTracker tracker(0.5);
    for (GameEvent& event : events) {
        double gameScore = PerformanceScorer::score(event);
        tracker.onGameEvent(event, gameScore);
    }

    for (auto& [playerId, fairValue] : tracker.allFairValues()) {
        std::cout << "PlayerID: " << playerId << ", Fair Value: " << fairValue << std::endl;
    }
    return 0;
}