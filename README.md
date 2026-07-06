# High-Throughput Limit Order Book for Player Performance Derivatives

A trading-systems demo C++ project: NBA players are modeled as tradable instruments, priced off real
performance data, and (eventually) matched through a limit order book. Developed using Vim. No LLM-generated code.

## Concept

- Each **player** has a **Fair Value (FV)** derived from their on-court performance, updated after
  each game via an EWMA (exponentially weighted moving average) over a per-game performance score.
- FV is converted into a **price** via a scaling function, so players behave like priced instruments.
- **Teams**, **divisions**, and **conferences** are modeled as **indices** — weighted aggregates of
  their constituents (players, or other indices) — analogous to how a team is an "index fund" of its
  roster, and a conference is an index of its divisions.
- (Planned) An **order book** will allow simulated buy/sell orders to be matched against each other,
  with market-making bots quoting around each instrument's Fair Value — creating a live, traded
  market price that is *informed by* but *independent from* Fair Value.

## Architecture

The project is deliberately split into two decoupled systems:

1. **Pricing Engine** (current focus) — computes Fair Value and price from historical box score data.
   Runs only when new game data is available (event-driven, not always-on).
2. **Order Book / Matching Engine** (not yet built) — always-on, latency-critical service that
   matches buy/sell orders independent of how prices are derived.

These are kept separate on purpose: the order book should work correctly regardless of what feeds
it prices (NBA players, or any other instrument), and the pricing engine should be independently
testable without any notion of orders or trading.

## Pricing Engine — components

| File | Responsibility |
|---|---|
| `GameEvent` | Plain data struct — one player's box score line for one game |
| `PerformanceScorer` | Stateless: `GameEvent -> Game Score` (based on Hollinger's Game Score formula) |
| `FairValueTracker` | Stateful: applies EWMA per player, `FV_n = alpha * score + (1-alpha) * FV_{n-1}` |
| `PriceScaler` | Stateless: converts an FV number into a dollar price |
| `PriceProvider` | Interface implemented by both `Player` and `Index` — `getPrice()`, `getId()`, `getName()` |
| `Player` | One NBA player; delegates to `FairValueTracker`/`PriceScaler`, implements `PriceProvider` |
| `Index` | A team/division/conference; holds a weighted list of `PriceProvider*` constituents (players or other indices) and aggregates their prices |
| `DataLoader` | Reads historical box scores from CSV into `GameEvent`s |

### Design notes

- **EWMA over a simple moving average**: only needs to store one running value per player (O(1)
  memory), no need to retain full game history.
- **Fixed-point-style pricing considerations**: prices are calculated as doubles and rounded to
  cents for display; a production version would consider integer/tick-based pricing to avoid
  floating point rounding issues in the order book.
- **Ownership model**: `Player` and `Index` objects are owned centrally (`std::unique_ptr`, keyed by
  ID) in `main`. `Index` does not own its constituents — it holds non-owning raw pointers via the
  `PriceProvider*` interface, so an `Index` can transparently contain `Player`s or other `Index`es
  without special-casing either.
- **Data source**: historical box scores only (current NBA season is off-season at time of writing).
  A Python script is used as a one-time offline ETL step to pull data into a local CSV — the C++
  engine only ever reads from that local file, no live/network dependency at runtime.

## Building and running

```bash
cd pricing_engine
mkdir -p build && cd build
cmake ..
make
./pricing_engine
```

Unit tests (Catch2):

```bash
ctest --output-on-failure
# or: ./tests/tests
```

## Current status

- [x] `GameEvent`, `PerformanceScorer`, `FairValueTracker`, `PriceScaler` implemented and unit tested
- [x] CSV `DataLoader` reading historical box scores
- [x] `Player` and `Index` (`PriceProvider` interface) wired together
- [x] End-to-end replay: CSV → Game Score → EWMA Fair Value → scaled price, aggregated into
      team-level indices
- [ ] Division / conference level indices (indices-of-indices)
- [ ] IPO pricing for rookies with no game history
- [ ] Persisted/snapshot output (optional)

---

## Order Book (planned — not yet started)

This section will be filled in as the matching engine is designed and built. Planned scope:

- Limit order book with price-time priority (limit orders, market orders, cancels, amends)
- Data structure selection or built and defended with a rationale and benchmarked performance
  (e.g. `std::map` price levels vs. flat array/hashmap of ticks)
- Integer/tick-based pricing to avoid floating point issues
- Market maker bot(s) quoting around each instrument's Fair Value (bridging the pricing engine and
  the book)
- Noise/momentum trader bots to generate realistic order flow
- Event sourcing / replayable order log for deterministic testing
- Threading model: single-threaded matching engine for determinism, separate thread for market data
  ingestion, communicating via a lock-free queue
- Latency/throughput benchmarks (p50/p99 for insert, cancel, match)
- (Stretch) networking layer exposing the engine over sockets/websocket for a simple client/dashboard
