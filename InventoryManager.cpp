#include "InventoryManager.h"
#include "FoodProduct.h"
#include "ClothingProduct.h"
#include "ElectronicsProduct.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// Helper to split a string by delimiter
static std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool InventoryManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // If the file doesn't exist, we will create it empty
        std::ofstream newFile(filename);
        return true; 
    }

    products.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> tokens = splitString(line, ',');
        if (tokens.size() < 5) continue;

        std::string type = tokens[0];
        try {
            int id = std::stoi(tokens[1]);
            std::string name = tokens[2];
            double basePrice = std::stod(tokens[3]);
            int stock = std::stoi(tokens[4]);

            if (type == "Food" && tokens.size() >= 6) {
                int expiryDays = std::stoi(tokens[5]);
                products.push_back(std::make_unique<FoodProduct>(id, name, basePrice, stock, expiryDays));
            } else if (type == "Clothing" && tokens.size() >= 7) {
                std::string size = tokens[5];
                std::string material = tokens[6];
                products.push_back(std::make_unique<ClothingProduct>(id, name, basePrice, stock, size, material));
            } else if (type == "Electronics" && tokens.size() >= 6) {
                int warrantyMonths = std::stoi(tokens[5]);
                products.push_back(std::make_unique<ElectronicsProduct>(id, name, basePrice, stock, warrantyMonths));
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line: " << line << " (" << e.what() << ")" << std::endl;
        }
    }
    file.close();
    return true;
}

bool InventoryManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    for (const auto& product : products) {
        file << product->serialize() << "\n";
    }
    file.close();
    return true;
}

bool InventoryManager::addProduct(std::unique_ptr<Product> product) {
    if (!product) return false;
    // Check for duplicate ID
    for (const auto& p : products) {
        if (p->getId() == product->getId()) {
            return false; // Duplicate ID not allowed
        }
    }
    products.push_back(std::move(product));
    return true;
}

bool InventoryManager::deleteProduct(int id) {
    auto it = std::remove_if(products.begin(), products.end(),
        [id](const std::unique_ptr<Product>& p) { return p->getId() == id; });
    
    if (it != products.end()) {
        products.erase(it, products.end());
        return true;
    }
    return false;
}

Product* InventoryManager::getProduct(int id) {
    for (auto& p : products) {
        if (p->getId() == id) {
            return p.get();
        }
    }
    return nullptr;
}

void InventoryManager::sortProducts(int choice) {
    switch (choice) {
        case 1: // Sort by ID Ascending
            std::sort(products.begin(), products.end(),
                [](const auto& a, const auto& b) { return a->getId() < b->getId(); });
            break;
        case 2: // Sort by Price (Discounted) Ascending
            std::sort(products.begin(), products.end(),
                [](const auto& a, const auto& b) { return a->getDiscountedPrice() < b->getDiscountedPrice(); });
            break;
        case 3: // Sort by Stock Descending
            std::sort(products.begin(), products.end(),
                [](const auto& a, const auto& b) { return a->getStock() > b->getStock(); });
            break;
        default:
            break;
    }
}
