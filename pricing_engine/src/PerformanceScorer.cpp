#include <GameEvent.h>
#include <PerformanceScorer.h>

// Game Score is a player evaluation metric which can be considered John Hollinger‘s 
// simple and linear version of the Player Efficiency Rating.

double PerformanceScorer::score(const GameEvent& event) const {
    int points                          = event.points;
    double weightedFieldGoalsMade       = event.fieldGoalsMade * 0.4;
    double weightedFieldGoalsAttempted  = event.fieldGoalsAttempted * 0.7;
    double weightedOffensiveRebounds    = event.offensiveRebounds * 0.7;
    double weightedDefensiveRebounds    = event.defensiveRebounds * 0.3;
    int steals                          = event.steals;
    double weightedAssists              = event.assists * 0.7;
    double weightedBlockedShots         = event.blockedShots * 0.7;
    double weightedFreeThrowsMissed     = event.freeThrowsMissed * 0.4;
    double weightedPersonalFouls        = event.personalFouls * 0.4;
    int turnovers                       = event.turnovers;
    
    double gameScore = points + weightedFieldGoalsMade + weightedOffensiveRebounds + weightedDefensiveRebounds 
    + steals + weightedAssists + weightedBlockedShots - weightedFieldGoalsAttempted - weightedFreeThrowsMissed 
    - weightedPersonalFouls - turnovers;
}