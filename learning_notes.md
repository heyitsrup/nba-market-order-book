## 2026-07-02
- Used work done in [previous project](https://github.com/heyitsrup/uniswap-data-engine/blob/main/data_engine/ingestion/data_loader.cpp) to create `DataLoader.cpp`
- Header files all stored under /include and these are not the same as Java interfaces, even though both can contain method declarations without implementations.
- `static` modifier means "this method can be accessed and used without having to instantiate this class as an object"
- `#pragma once` is a non-standard but widely supported preprocessor directive designed to cause the current source file to be included only once in a single compilation.
- `std::` is still used to reduce global scope pollution & increases risk of name collisions.
- `auto` keyword automatically detects the type of a variable based on the value you assign to it. Helpful if return type of a function might be changed later 

- `PerformanceScorer.cpp` calculates [game score](https://www.nbastuffer.com/analytics101/game-score/) for given game event - each event being for single player.

## 2026-07-03
- Need to use unit testing framework; downloaded CMake and Catch2
- .cpp file must be added to root `CMakeLists.txt` file under add_library
- No easy way to return float values to x number of digits -- use tolerance unit tests instead
- C++ doesn't support Python-style named field initialization
- `std::map<X, Y>`, what it actually stores is a whole bunch of `std::pair<const X, Y>`s.
- `std::pair<X, Y>` called p, `p.first` is an X object and `p.second` is a Y object.
- `override` only appears in the header's declaration, not in the `.cpp` definition - the header is where it matters for the compiler to check against the base class
- the pointer * points to the address location of the data. The reference & points to the data.
- override also requires implementation in .cpp file
- can use explicit in header for constructor (like in Index.hpp) if single argument
- unique_ptr means single, clear owner of an object, making ownership explicit and preventing memory leaks.