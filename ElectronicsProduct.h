#pragma once
#include "Product.h"

class ElectronicsProduct : public Product {
private:
    int warrantyMonths;

public:
    ElectronicsProduct(int id, const std::string& name, double basePrice, int stock, int warrantyMonths)
        : Product(id, name, basePrice, stock), warrantyMonths(warrantyMonths) {}

    // Electronics price logic: Adds a 5% warranty/handling fee (multiplied by 1.05)
    double getDiscountedPrice() const override {
        return basePrice * 1.05;
    }

    std::string getProductType() const override {
        return "Electronics";
    }

    int getWarrantyMonths() const { return warrantyMonths; }

    void displayInfo() const override {
        Product::displayInfo();
        std::cout << std::left << "Warranty: " << warrantyMonths << " months" << std::endl;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << Product::serialize() << "," << warrantyMonths;
        return ss.str();
    }
};
