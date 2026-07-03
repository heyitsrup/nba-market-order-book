#pragma once

#include <GameEvent.hpp>

class PerformanceScorer {
public:
    double score(const GameEvent& event) const;
};