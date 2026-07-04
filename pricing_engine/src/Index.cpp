#include <PriceProvider.hpp>
#include <string>
#include <Index.hpp>

Index::Index(std::string teamTicker, std::vector<std::pair<PriceProvider*, double>> assets) 
    : teamTicker(std::move(teamTicker)), assets(std::move(assets)) {}

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

std::string Index::getId() const {
    return teamTicker;
}

std::vector<std::pair<PriceProvider*, double>> Index::getAllAssets() {
    return assets;
}