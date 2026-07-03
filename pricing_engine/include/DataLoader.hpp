#pragma once
#include <vector>
#include <GameEvent.hpp>
#include <string>

class DataLoader {
private:
    static int parseInt(const std::string& s);
    static std::string trim(const std::string& str);
    static std::vector<std::string> splitCsvLine(const std::string& line);
public:
    std::vector<GameEvent> loadEvents(const std::string& filename);
};