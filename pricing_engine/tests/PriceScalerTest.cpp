#include <catch2/catch_test_macros.hpp>
#include "PriceScaler.hpp"

TEST_CASE("Price Scaler - successful test high value", "[PriceScaler]") {
    double fairValue = 25;
    REQUIRE(PriceScaler::toPrice(fairValue) == 219.76);
}

TEST_CASE("Price Scaler - successful test medium value", "[PriceScaler]") {
    double fairValue = 15;
    REQUIRE(PriceScaler::toPrice(fairValue) == 83.36);
}

TEST_CASE("Price Scaler - successful test low value", "[PriceScaler]") {
    double fairValue = 5;
    REQUIRE(PriceScaler::toPrice(fairValue) == 15.16);
}

TEST_CASE("Price Scaler - successful test negative value", "[PriceScaler]") {
    double fairValue = -0.5;
    REQUIRE(PriceScaler::toPrice(fairValue) > 0);
}