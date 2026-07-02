#pragma once
#include <vector>
#include <GameEvent.h>
#include <string>

class DataLoader {
public:
    std::vector<GameEvent> loadEvents(const std::string& filename);
};