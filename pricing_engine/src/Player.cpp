#include <PriceProvider.hpp>
#include <string>
#include <Player.hpp>

Player::Player(std::string playerId, std::string playerName, std::string teamTicker, FairValueTracker &tracker, PriceScaler &scaler)
    : playerId(std::move(playerId)), playerName(std::move(playerName)), teamTicker(std::move(teamTicker)), tracker(tracker), scaler(scaler) {}

void Player::onGameEvent(const GameEvent &event, double gameScore)
{
    tracker.onGameEvent(event, gameScore);
}

double Player::getPrice() const
{
    double currentFairValue = tracker.getFairValue(playerId);
    return scaler.toPrice(currentFairValue);
}

std::string Player::getId() const
{
    return playerId;
}

std::string Player::getName() const
{
    return playerName;
}
