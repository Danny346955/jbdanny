#pragma once
#include <vector>
#include <memory>
#include "Product.h"

class InventoryManager {
private:
    std::vector<std::unique_ptr<Product>> products;

public:
    InventoryManager() = default;

    // Load and Save files
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    // Add & Remove items
    bool addProduct(std::unique_ptr<Product> product);
    bool deleteProduct(int id);

    // Getters & Helper functions
    Product* getProduct(int id);
    const std::vector<std::unique_ptr<Product>>& getProducts() const { return products; }

    // Sorting functions (using STL std::sort)
    void sortProducts(int choice);
};
