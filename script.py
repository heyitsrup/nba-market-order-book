from nba_api.stats.endpoints import LeagueGameFinder, BoxScoreTraditionalV3
import time
from requests.exceptions import ReadTimeout

season = "2025-26"

def fetch_games(season):
    gamefinder = LeagueGameFinder(season_nullable=season)

    df = gamefinder.get_data_frames()[0]
    game_ids_deduped = df['GAME_ID'].drop_duplicates().tolist()

    return game_ids_deduped

def fetch_boxscores(game_id, retries=5):
    for i in range(retries):
        try:
            return BoxScoreTraditionalV3(game_id=game_id, timeout=60).get_data_frames()[0]
        except ReadTimeout:
            wait = 2 ** 1
            print(f"Timeout. Retrying in {wait}s...")
            time.sleep(wait)
    
    raise Exception("Failed after retries")

if __name__ == "__main__":
    file = 'pricing_engine/data/boxscores.csv'
    for game_id in fetch_games(season):
        current_boxscore = fetch_boxscores(game_id=game_id)
        current_boxscore.to_csv(file, mode="a", header=False, index=False)
        time.sleep(1.5)
