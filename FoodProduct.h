#pragma once
#include "Product.h"

class FoodProduct : public Product {
private:
    int expiryDays;

public:
    FoodProduct(int id, const std::string& name, double basePrice, int stock, int expiryDays)
        : Product(id, name, basePrice, stock), expiryDays(expiryDays) {}

    // Expiry-based discount logic:
    // Expiry <= 3 days: 50% off
    // Expiry <= 7 days: 20% off
    // Otherwise: No discount (100% price)
    double getDiscountedPrice() const override {
        if (expiryDays <= 3) {
            return basePrice * 0.5;
        } else if (expiryDays <= 7) {
            return basePrice * 0.8;
        }
        return basePrice;
    }

    std::string getProductType() const override {
        return "Food";
    }

    int getExpiryDays() const { return expiryDays; }

    void displayInfo() const override {
        Product::displayInfo();
        std::cout << std::left << "Expiry: " << expiryDays << " days" << std::endl;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << Product::serialize() << "," << expiryDays;
        return ss.str();
    }
};
