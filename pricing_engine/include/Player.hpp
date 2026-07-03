#pragma once

#include <PriceProvider.hpp>
#include <GameEvent.hpp>
#include <string>
#include <FairValueTracker.hpp>
#include <PriceScaler.hpp>

class Player : public PriceProvider {
public:
    void onGameEvent(const GameEvent& event);
    double getPrice() const override;
    std::string getId() const override;
private:
    std::string playerId;
    FairValueTracker& tracker;   
    PriceScaler& scaler;
};