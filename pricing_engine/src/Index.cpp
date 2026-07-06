#include <Index.hpp>
#include <PriceProvider.hpp>
#include <string>

Index::Index(std::string teamTicker) : teamTicker(std::move(teamTicker)) {}

void Index::addAsset(PriceProvider* provider, double weight) {
    assets.emplace_back(provider, weight);
}

double Index::getPrice() const {
    double total = 0.0;
    for (auto& [provider, weight] : assets) {
        total += provider->getPrice() * weight;
    }
    return total;
}

std::string Index::getName() const {
    return teamTicker;
}

std::string Index::getId() const {
    return teamTicker;
}

std::vector<std::pair<PriceProvider*, double>> Index::getAllAssets() const {
    return assets;
}