#pragma once

#include "GameEvent.hpp"
#include <string>
#include <unordered_map>

class FairValueTracker {
public:
    explicit FairValueTracker(double alpha,
                              std::unordered_map<std::string, double> initialValues = {});
    void onGameEvent(const GameEvent& event, double gameScore);
    double getFairValue(const std::string& playerId) const;
    const std::unordered_map<std::string, double>& allFairValues() const;

private:
    std::unordered_map<std::string, double> fairValues;
    double alpha;
};