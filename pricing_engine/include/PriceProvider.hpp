#pragma once

#include <string>

class PriceProvider {
public:
    virtual double getPrice() const = 0;
    virtual std::string getId() const = 0;
    virtual ~PriceProvider() = default;
};