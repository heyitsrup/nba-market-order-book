#pragma once

#include <PriceProvider.hpp>
#include <GameEvent.hpp>
#include <string>
#include <FairValueTracker.hpp>
#include <PriceScaler.hpp>

class Player : public PriceProvider {
public:
    Player(std::string playerId, FairValueTracker& tracker, PriceScaler& scaler);
    
    void onGameEvent(const GameEvent& event, double gameScore);
    double getPrice() const override;
    std::string getId() const override;
private:
    std::string playerId;
    FairValueTracker& tracker;   
    PriceScaler& scaler;
};