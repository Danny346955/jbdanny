#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

class Product {
protected:
    int id;
    std::string name;
    double basePrice;
    int stock;

public:
    Product(int id, const std::string& name, double basePrice, int stock)
        : id(id), name(name), basePrice(basePrice), stock(stock) {}

    virtual ~Product() = default;

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    double getBasePrice() const { return basePrice; }
    int getStock() const { return stock; }

    // Setters
    void setStock(int newStock) { stock = newStock; }

    // Pure virtual functions (Must be overridden by derived classes)
    virtual double getDiscountedPrice() const = 0;
    virtual std::string getProductType() const = 0;

    // Virtual display method
    virtual void displayInfo() const {
        std::cout << std::left 
                  << std::setw(12) << getProductType()
                  << std::setw(8) << id
                  << std::setw(18) << name
                  << std::setw(12) << basePrice
                  << std::setw(12) << getDiscountedPrice()
                  << std::setw(8) << stock;
    }

    // Virtual serialization method (Used to save to file)
    virtual std::string serialize() const {
        std::stringstream ss;
        ss << getProductType() << ","
           << id << ","
           << name << ","
           << basePrice << ","
           << stock;
        return ss.str();
    }
};
