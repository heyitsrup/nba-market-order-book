#include "FairValueTracker.hpp"

FairValueTracker::FairValueTracker(double alpha,
                                   std::unordered_map<std::string, double> initialValues)
    : fairValues(std::move(initialValues)), alpha(alpha) {}

void FairValueTracker::onGameEvent(const GameEvent& event, double gameScore) {
    const std::string& playerId = event.playerId;

    auto item = fairValues.find(playerId);
    if (item == fairValues.end()) {
        fairValues[playerId] = gameScore;
    } else {
        item->second = alpha * gameScore + (1.0 - alpha) * item->second;
    }
}

double FairValueTracker::getFairValue(const std::string& playerId) const {
    auto it = fairValues.find(playerId);
    return it == fairValues.end() ? 0.0 : it->second;
}

const std::unordered_map<std::string, double>& FairValueTracker::allFairValues() const {
    return fairValues;
}