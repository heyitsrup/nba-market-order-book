import json
import logging
import os
import time

from nba_api.stats.endpoints import BoxScoreTraditionalV3, LeagueGameFinder
from requests.exceptions import ReadTimeout

season = "2025-26"

OUTPUT_FILE = "pricing_engine/data/boxscores.csv"
PROGRESS_FILE = "pricing_engine/data/progress.json"
LOG_FILE = "pricing_engine/data/fetch.log"

EXCLUDED_SEASON_TYPES = {"All-Star"}

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        logging.FileHandler(LOG_FILE),
        logging.StreamHandler(),
    ],
)
logger = logging.getLogger(__name__)


def load_progress():
    if os.path.exists(PROGRESS_FILE):
        with open(PROGRESS_FILE, "r") as f:
            data = json.load(f)
            return set(data.get("completed_game_ids", []))
    return set()


def save_progress(completed_ids):
    os.makedirs(os.path.dirname(PROGRESS_FILE), exist_ok=True)
    with open(PROGRESS_FILE, "w") as f:
        json.dump({"completed_game_ids": sorted(completed_ids)}, f)


def fetch_games(season):
    logger.info(f"Fetching game list for season {season}")
    gamefinder = LeagueGameFinder(season_nullable=season)
    df = gamefinder.get_data_frames()[0]

    if "SEASON_TYPE" in df.columns:
        before = len(df)
        df = df[~df["SEASON_TYPE"].isin(EXCLUDED_SEASON_TYPES)]
        excluded = before - len(df)
        if excluded:
            logger.info(f"Excluded {excluded} All-Star rows")

    wl_lookup = {}
    for _, row in df.iterrows():
        game_id = row["GAME_ID"]
        wl_lookup.setdefault(game_id, {})[row["TEAM_ID"]] = {
            "team_abbreviation": row.get("TEAM_ABBREVIATION"),
            "wl": row.get("WL"),
            "season_type": row.get("SEASON_TYPE", "Regular Season"),
        }

    game_ids_deduped = df["GAME_ID"].drop_duplicates().tolist()
    logger.info(f"Found {len(game_ids_deduped)} games to process")
    return game_ids_deduped, wl_lookup


def fetch_boxscores(game_id, retries=5):
    for i in range(retries):
        try:
            return BoxScoreTraditionalV3(game_id=game_id, timeout=60).get_data_frames()[0]
        except ReadTimeout:
            wait = 2 ** i
            logger.warning(f"Timeout on game {game_id}. Retrying in {wait}s... ({i + 1}/{retries})")
            time.sleep(wait)
        except Exception as e:
            logger.error(f"Unexpected error on game {game_id}: {e}")
            time.sleep(2 ** i)

    raise Exception(f"Failed to fetch game {game_id} after {retries} retries")


def annotate_boxscore(df, game_id, wl_lookup):
    game_info = wl_lookup.get(game_id, {})

    def team_wl(team_id):
        return game_info.get(team_id, {}).get("wl")

    def team_season_type(team_id):
        return game_info.get(team_id, {}).get("season_type", "Regular Season")

    if "teamId" in df.columns:
        df["WL"] = df["teamId"].map(team_wl)
        df["SEASON_TYPE"] = df["teamId"].map(team_season_type)
    elif "TEAM_ID" in df.columns:
        df["WL"] = df["TEAM_ID"].map(team_wl)
        df["SEASON_TYPE"] = df["TEAM_ID"].map(team_season_type)
    else:
        logger.warning(f"No team id column found for game {game_id}; WL/SEASON_TYPE not attached")

    return df


def main():
    os.makedirs(os.path.dirname(OUTPUT_FILE), exist_ok=True)
    completed_ids = load_progress()
    if completed_ids:
        logger.info(f"Resuming: {len(completed_ids)} games already completed, will be skipped")

    game_ids, wl_lookup = fetch_games(season)
    write_header = not os.path.exists(OUTPUT_FILE) or os.path.getsize(OUTPUT_FILE) == 0

    for idx, game_id in enumerate(game_ids, start=1):
        if game_id in completed_ids:
            continue

        logger.info(f"[{idx}/{len(game_ids)}] Fetching boxscore for game {game_id}")
        try:
            current_boxscore = fetch_boxscores(game_id=game_id)
            current_boxscore = annotate_boxscore(current_boxscore, game_id, wl_lookup)

            current_boxscore.to_csv(
                OUTPUT_FILE, mode="a", header=write_header, index=False
            )
            write_header = False

            completed_ids.add(game_id)
            save_progress(completed_ids)
        except Exception as e:
            logger.error(f"Giving up on game {game_id}: {e}")
            save_progress(completed_ids)
            continue

        time.sleep(1.5)

    logger.info(f"Done. {len(completed_ids)}/{len(game_ids)} games processed.")


if __name__ == "__main__":
    main()
