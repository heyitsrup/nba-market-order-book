#pragma once

#include <PriceProvider.hpp>
#include <string>
#include <vector>

class Index : public PriceProvider {
public:
    explicit Index(std::string teamTicker);
    void addAsset(PriceProvider* provider, double weight);
    double getPrice() const override;
    std::string getId() const override;
    std::string getName() const override;
    std::vector<std::pair<PriceProvider*, double>> getAllAssets() const;

private:
    std::vector<std::pair<PriceProvider*, double>> assets;
    std::string teamTicker;
};