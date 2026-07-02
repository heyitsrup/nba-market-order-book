#include <map>
#include <string>
#include <GameEvent.h>

class FairValueTracker {
public:
    void onGameEvent(const GameEvent& event, double gameScore);
    double getFairValue(const std::string& playerId) const;
private:
    std::unordered_map<std::string, double> fairValues;
    double alpha;
};