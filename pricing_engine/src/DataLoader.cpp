#pragma once
#include <vector>
#include <GameEvent.h>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

static int64_t parse_int(const std::string& s) {
    if (s.empty()) return 0;
    try { return stoi(s); } catch (...) { return 0; }
}

static std::string trim(const std::string& str) {
    if (str.empty()) return "";
    size_t first = str.find_first_not_of(" \t\r\n\"");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n\"");
    return str.substr(first, last - first + 1);
}

static std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string field;
    while (getline(ss, field, ',')) {
        result.push_back(trim(field));
    }
    return result;
}

std::vector<GameEvent> loadEvents(const std::string& filename) {
    std::vector<GameEvent> events;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[pricing_engine] Error: Cannot open boxscores file:" << filename << std::endl;
        return events;
    }

    std::string line;
    int lineNum = 1;
    while (getline(file, line)) {
        if (line.empty()) continue;

        auto fields = splitCsvLine(line);
        if (fields.size() != 33) {
            std::cerr << "[pricing_engine] Malformed row at line " << lineNum
                 << " (got " << fields.size() << " fields, expected 11)\n";
            continue;
        }
        GameEvent game;
        try {
            game.playerId               = fields[6];
            game.gameId                 = fields[0];
            game.points                 = parse_int(fields[32]);
            game.fieldGoalsMade         = parse_int(fields[15]);
            game.fieldGoalsAttempted    = parse_int(fields[16]);
            game.offensiveRebounds      = parse_int(fields[24]);
            game.defensiveRebounds      = parse_int(fields[25]);
            game.steals                 = parse_int(fields[28]);
            game.assists                = parse_int(fields[27]);
            game.blockedShots           = parse_int(fields[29]);
            game.personalFouls          = parse_int(fields[31]);
            game.turnovers              = parse_int(fields[30]);
        } catch (const std::exception& e) {
            std::cerr << "[ingestion] Exception parsing swap at line " << lineNum
                 << ": " << e.what() << "\n";
            continue;
        }
    }

    file.close();
    return events;
}
