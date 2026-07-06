#pragma once
#include <GameEvent.hpp>
#include <stdexcept>
#include <string>
#include <vector>

class DataLoaderFileException : public std::runtime_error {
public:
    explicit DataLoaderFileException(const std::string& msg) : std::runtime_error(msg) {}
};

class DataLoader {
private:
    static int parseInt(const std::string& s);
    static std::string trim(const std::string& str);
    static std::vector<std::string> splitCsvLine(const std::string& line);

public:
    static std::vector<GameEvent> loadEvents(const std::string& filename);
};