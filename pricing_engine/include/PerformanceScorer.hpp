#pragma once

#include <GameEvent.hpp>

class PerformanceScorer {
public:
    static double score(const GameEvent& event);
};