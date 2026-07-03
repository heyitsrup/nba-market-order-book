#include <DataLoader.hpp>
#include <vector>
#include <GameEvent.hpp>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int DataLoader::parseInt(const std::string &s)
{
    if (s.empty())
        return 0;
    try
    {
        // C++ standard library function used to convert a String TO Integer
        return stoi(s);
    }
    catch (...)
    {
        return 0;
    }
}

std::string DataLoader::trim(const std::string &str)
{
    if (str.empty())
        return "";
    size_t first = str.find_first_not_of(" \t\r\n\"");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n\"");
    return str.substr(first, last - first + 1);
}

std::vector<std::string> DataLoader::splitCsvLine(const std::string &line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string field;
    while (getline(ss, field, ','))
    {
        result.push_back(trim(field));
    }
    return result;
}

std::vector<GameEvent> DataLoader::loadEvents(const std::string &filename)
{
    std::vector<GameEvent> events;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "[pricing_engine] Error: Cannot open boxscores file:" << filename << std::endl;
        return events;
    }

    std::string line;
    int lineNum = 1;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        auto fields = splitCsvLine(line);
        if (fields.size() != 33)
        {
            std::cerr << "[pricing_engine] Malformed row at line " << lineNum
                      << " (got " << fields.size() << " fields, expected 11)\n";
            continue;
        }
        GameEvent game;
        try
        {
            game.playerId = fields[6];
            game.gameId = fields[0];
            game.points = parseInt(fields[32]);
            game.fieldGoalsMade = parseInt(fields[15]);
            game.fieldGoalsAttempted = parseInt(fields[16]);
            game.offensiveRebounds = parseInt(fields[24]);
            game.defensiveRebounds = parseInt(fields[25]);
            game.steals = parseInt(fields[28]);
            game.assists = parseInt(fields[27]);
            game.blockedShots = parseInt(fields[29]);
            game.personalFouls = parseInt(fields[31]);
            game.turnovers = parseInt(fields[30]);
            game.freeThrowsMissed = parseInt(fields[22]) - parseInt(fields[21]);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[pricing_engine] Exception parsing game event at line " << lineNum
                      << ": " << e.what() << "\n";
            continue;
        }
    }

    file.close();
    return events;
}
