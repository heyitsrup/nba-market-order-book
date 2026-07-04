#include "PriceScaler.hpp"
#include <cmath>
#include <algorithm>

double PriceScaler::toPrice(double fairValue) {
    const double k = 0.341;
    const double base = 6.63;
    double clampedFV = std::max(fairValue, 0.0);
    double price = base + k * clampedFV * clampedFV;
    return std::round(price * 100.0) / 100.0;
}