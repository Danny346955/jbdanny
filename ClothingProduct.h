#pragma once
#include "Product.h"

class ClothingProduct : public Product {
private:
    std::string size;
    std::string material;

public:
    ClothingProduct(int id, const std::string& name, double basePrice, int stock, const std::string& size, const std::string& material)
        : Product(id, name, basePrice, stock), size(size), material(material) {}

    // Clothing discount logic: 10% off seasonal promotion
    double getDiscountedPrice() const override {
        return basePrice * 0.9;
    }

    std::string getProductType() const override {
        return "Clothing";
    }

    std::string getSize() const { return size; }
    std::string getMaterial() const { return material; }

    void displayInfo() const override {
        Product::displayInfo();
        std::cout << std::left << "Size: " << size << ", Material: " << material << std::endl;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << Product::serialize() << "," << size << "," << material;
        return ss.str();
    }
};
