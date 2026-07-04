#pragma once

#include <PriceProvider.hpp>
#include <string>
#include <vector>

class Index : public PriceProvider {
public:
    Index(std::string teamTicker, std::vector<std::pair<PriceProvider*, double>> assets);
    void addAsset(PriceProvider* provider, double weight); 
    double getPrice() const override; 
    std::string getId() const override;
    std::vector<std::pair<PriceProvider*, double>> getAllAssets();
private:
    std::vector<std::pair<PriceProvider*, double>> assets;
    std::string teamTicker;
};