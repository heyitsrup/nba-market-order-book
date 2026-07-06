#include "DataLoader.hpp"
#include "FairValueTracker.hpp"
#include "GameEvent.hpp"
#include "Index.hpp"
#include "PerformanceScorer.hpp"
#include "Player.hpp"
#include "PriceScaler.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::string filename = "/Users/rup/Git/nba-market-order-book/pricing_engine/data/boxscores.csv";
    auto events = DataLoader::loadEvents(filename);

    std::unordered_map<std::string, std::unique_ptr<Player>> allPlayers;
    std::unordered_map<std::string, std::unique_ptr<Index>> allIndexes;
    double alpha = alpha = 1 - std::pow(0.5, (1 / 12.5)); // Average number of monthly games played
    FairValueTracker tracker(alpha);
    PriceScaler scaler;

    for (GameEvent& event : events) {
        std::string playerId = event.playerId;
        double gameScore = PerformanceScorer::score(event);

        auto playerIt = allPlayers.find(playerId);
        if (playerIt == allPlayers.end()) {
            auto [newIt, ok] = allPlayers.emplace(
                playerId,
                std::make_unique<Player>(
                    playerId, event.playerName, event.teamTicker, tracker, scaler));
            playerIt = newIt;

            std::string teamTicker = playerIt->second->getTeamTicker();
            auto indexIt = allIndexes.find(teamTicker);
            if (indexIt == allIndexes.end()) {
                auto [newIndexIt, ok2] =
                    allIndexes.emplace(teamTicker, std::make_unique<Index>(teamTicker));
                indexIt = newIndexIt;
            }
            indexIt->second->addAsset(playerIt->second.get(), 1.0 / 15);
        } else {
            playerIt->second->onGameEvent(event, gameScore);
        }
    }

    for (const auto& [key, index] : allIndexes) {
        std::string team = key;
        for (const auto& [provider, player] : index->getAllAssets()) {
            std::cout << "Player: " << provider->getName() << " (" << team << "), Player Price: $"
                      << provider->getPrice() << std::endl;
        }
    }

    return 0;
}