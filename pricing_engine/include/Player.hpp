#pragma once

#include <FairValueTracker.hpp>
#include <GameEvent.hpp>
#include <PriceProvider.hpp>
#include <PriceScaler.hpp>
#include <string>

class Player : public PriceProvider {
public:
    Player(std::string playerId,
           std::string playerName,
           std::string teamTicker,
           FairValueTracker& tracker,
           PriceScaler& scaler);

    void onGameEvent(const GameEvent& event, double gameScore);
    double getPrice() const override;
    std::string getId() const override;
    std::string getName() const override;
    std::string getTeamTicker() const;

private:
    std::string playerId;
    std::string playerName;
    std::string teamTicker;
    FairValueTracker& tracker;
    PriceScaler& scaler;
};