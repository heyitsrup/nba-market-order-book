#include "DataLoader.hpp"
#include "GameEvent.hpp"
#include "PerformanceScorer.hpp"
#include "FairValueTracker.hpp"
#include "PriceScaler.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include "Player.hpp"
#include "Index.hpp"

int main()
{
    std::string filename = "/Users/rup/Git/nba-market-order-book/pricing_engine/data/boxscores.csv";
    auto events = DataLoader::loadEvents(filename);

    std::unordered_map<std::string, std::unique_ptr<Player>> allPlayers;
    std::unordered_map<std::string, std::unique_ptr<Index>> allIndexes;
    FairValueTracker tracker(0.5);
    PriceScaler scaler;

    for (GameEvent &event : events)
    {
        std::string playerId = event.playerId;
        double gameScore = PerformanceScorer::score(event);

        auto player = allPlayers.find(playerId);
        if (player == allPlayers.end())
        {
            allPlayers[playerId] = std::make_unique<Player>(event.playerId, event.playerName, event.teamTicker, tracker, scaler);
            std::string teamTicker = player->second->getTeamTicker();
            auto teamIndex = allIndexes.find(teamTicker);
            if (teamIndex == allIndexes.end())
            {
                allIndexes[teamTicker] = std::make_unique<Index>(event.teamTicker);
            } else {
                teamIndex->second->addAsset(player->second.get(), 1/15);
            }
        }
        else
        {
            player->second->onGameEvent(event, gameScore);
            std::string teamTicker = player->second->getTeamTicker();
            auto teamIndex = allIndexes.find(teamTicker);
            teamIndex->second->addAsset(player->second.get(), 1/15); 
        }
    }

    for (const auto &[key, index] : allIndexes)
    {
        std::string team = key;
        for (const auto &[key, player] : index->getAllAssets()) {
            std::cout << "Player: " << player->getName() << " (" <<  team << "), Player Price: $" << player->getPrice() << std::endl;
        }
    }

    return 0;
}