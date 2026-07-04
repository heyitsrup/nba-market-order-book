#include <PriceProvider.hpp>
#include <string>
#include <Player.hpp>

Player::Player(std::string playerId, FairValueTracker& tracker, PriceScaler& scaler)
    : playerId(std::move(playerId)), tracker(tracker), scaler(scaler) {}

void Player::onGameEvent(const GameEvent& event, double gameScore) {
    tracker.onGameEvent(event, gameScore);
}

double Player::getPrice() const {
    double currentFairValue = tracker.getFairValue(playerId);
    return scaler.toPrice(currentFairValue);
}

std::string Player::getId() const {
    return playerId;
}
