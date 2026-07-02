#pragma once

#include <GameEvent.h>

class PerformanceScorer {
public:
    double score(const GameEvent& event) const;
};