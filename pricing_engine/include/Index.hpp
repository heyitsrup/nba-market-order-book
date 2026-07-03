#pragma once

#include <PriceProvider.hpp>
#include <string>
#include <vector>

class Index : public PriceProvider {
public:
    void addConstituent(PriceProvider* provider, double weight);
    double getPrice() const override; // weighted sum of constituents
    std::string getId() const override;
private:
    std::vector<std::pair<PriceProvider*, double>> constituents;
};