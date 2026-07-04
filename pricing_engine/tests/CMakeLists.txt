include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1   # pin to a specific release
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests 
  DataLoaderTest.cpp 
  PerformanceScorerTest.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE pricing_engine_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)