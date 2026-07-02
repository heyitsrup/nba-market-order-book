from nba_api.stats.endpoints import LeagueGameFinder, BoxScoreTraditionalV3

season = "2025-26"

gamefinder = LeagueGameFinder(season_nullable=season)

df = gamefinder.get_data_frames()[0]
game_ids_deduped = df['GAME_ID'].drop_duplicates().tolist()

boxscores = []
for game_id in game_ids_deduped[:10]:
    current_boxscore = BoxScoreTraditionalV3(game_id=game_id).get_data_frames()[0]
    boxscores.append(current_boxscore)

print(boxscores[:10])