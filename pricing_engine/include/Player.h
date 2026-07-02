#pragma once

#include <PriceProvider.h>
#include <GameEvent.h>
#include <string>
#include <FairValueTracker.h>
#include <PriceScaler.h>

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