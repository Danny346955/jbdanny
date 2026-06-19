#include <iostream>
#include <limits>
#include <iomanip>
#include "InventoryManager.h"
#include "FoodProduct.h"
#include "ClothingProduct.h"
#include "ElectronicsProduct.h"

const std::string FILENAME = "inventory.txt";

// Helper function to safely get integer input
int getIntegerInput(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << "無效的輸入，請輸入整數。\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Helper function to safely get double input
double getDoubleInput(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << "無效的輸入，請輸入數值。\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Display table header
void displayTableHeader() {
    std::cout << "\n------------------------------------------------------------------------------------------\n";
    std::cout << std::left 
              << std::setw(12) << "商品類型"
              << std::setw(8) << "ID"
              << std::setw(18) << "名稱"
              << std::setw(12) << "原價"
              << std::setw(12) << "折實/計費"
              << std::setw(8) << "庫存"
              << "詳細屬性\n";
    std::cout << "------------------------------------------------------------------------------------------\n";
}

void printInventory(const InventoryManager& manager) {
    const auto& list = manager.getProducts();
    if (list.empty()) {
        std::cout << "\n[庫存清單為空]\n";
        return;
    }
    displayTableHeader();
    for (const auto& product : list) {
        product->displayInfo();
    }
    std::cout << "------------------------------------------------------------------------------------------\n";
}

void addProductUI(InventoryManager& manager) {
    std::cout << "\n--- 新增商品入庫 ---\n";
    std::cout << "選擇商品類型:\n";
    std::cout << "1) 食品類 (Food)\n";
    std::cout << "2) 服飾類 (Clothing)\n";
    std::cout << "3) 電子產品類 (Electronics)\n";
    int typeChoice = getIntegerInput("請輸入選項 (1-3): ");

    if (typeChoice < 1 || typeChoice > 3) {
        std::cout << "無效的商品類型選單。\n";
        return;
    }

    int id = getIntegerInput("請輸入商品唯一 ID: ");
    if (manager.getProduct(id) != nullptr) {
        std::cout << "錯誤：該商品 ID (" << id << ") 已經存在！\n";
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear newline character
    std::cout << "請輸入商品名稱: ";
    std::string name;
    std::getline(std::cin, name);

    double basePrice = getDoubleInput("請輸入商品原價: ");
    int stock = getIntegerInput("請輸入商品初始庫存: ");

    std::unique_ptr<Product> newProduct = nullptr;

    if (typeChoice == 1) {
        int expiryDays = getIntegerInput("請輸入食品有效天數 (Expiry Days): ");
        newProduct = std::make_unique<FoodProduct>(id, name, basePrice, stock, expiryDays);
    } else if (typeChoice == 2) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "請輸入尺寸 (Size, 如 S, M, L): ";
        std::string size;
        std::getline(std::cin, size);
        std::cout << "請輸入材質 (Material, 如 Cotton, Denim): ";
        std::string material;
        std::getline(std::cin, material);
        newProduct = std::make_unique<ClothingProduct>(id, name, basePrice, stock, size, material);
    } else if (typeChoice == 3) {
        int warrantyMonths = getIntegerInput("請輸入電子產品保固月數 (Warranty Months): ");
        newProduct = std::make_unique<ElectronicsProduct>(id, name, basePrice, stock, warrantyMonths);
    }

    if (newProduct && manager.addProduct(std::move(newProduct))) {
        std::cout << "\n[系統訊息] 商品成功新增至庫存！\n";
    } else {
        std::cout << "\n[系統訊息] 新增商品失敗。\n";
    }
}

void deleteProductUI(InventoryManager& manager) {
    std::cout << "\n--- 刪除商品 ---\n";
    int id = getIntegerInput("請輸入要刪除的商品 ID: ");
    if (manager.deleteProduct(id)) {
        std::cout << "\n[系統訊息] 商品 (ID: " << id << ") 已成功從庫存中移除！\n";
    } else {
        std::cout << "\n[系統訊息] 找不到該商品 ID，刪除失敗。\n";
    }
}

void sortProductsUI(InventoryManager& manager) {
    std::cout << "\n--- 商品排序選項 ---\n";
    std::cout << "1) 按商品 ID 升序排序\n";
    std::cout << "2) 按最終售價（折實/計費）升序排序\n";
    std::cout << "3) 按商品庫存量降序排序\n";
    int choice = getIntegerInput("請選擇排序規則 (1-3): ");
    
    if (choice >= 1 && choice <= 3) {
        manager.sortProducts(choice);
        std::cout << "\n[系統訊息] 商品排序完成！\n";
        printInventory(manager);
    } else {
        std::cout << "\n[系統訊息] 無效的排序選項。\n";
    }
}

void posCheckoutUI(InventoryManager& manager) {
    std::cout << "\n=============================================\n";
    std::cout << "         模擬收銀結帳系統 (POS Simulator)    \n";
    std::cout << "=============================================\n";
    
    struct CartItem {
        Product* product;
        int quantity;
    };
    std::vector<CartItem> cart;

    while (true) {
        int id = getIntegerInput("輸入要購買的商品 ID (輸入 0 結束選購): ");
        if (id == 0) break;

        Product* p = manager.getProduct(id);
        if (!p) {
            std::cout << "找不到該商品！\n";
            continue;
        }

        std::cout << "選中商品: " << p->getName() 
                  << " (類型: " << p->getProductType() 
                  << ", 目前庫存: " << p->getStock() 
                  << ", 最終單價: " << p->getDiscountedPrice() << ")\n";

        int qty = getIntegerInput("請輸入購買數量: ");
        if (qty <= 0) {
            std::cout << "購買數量必須大於 0！\n";
            continue;
        }

        // Check stock
        int currentInCart = 0;
        for (const auto& item : cart) {
            if (item.product->getId() == id) {
                currentInCart += item.quantity;
            }
        }

        if (p->getStock() < currentInCart + qty) {
            std::cout << "庫存不足！尚可購買數量: " << (p->getStock() - currentInCart) << "\n";
            continue;
        }

        // Add to cart
        bool found = false;
        for (auto& item : cart) {
            if (item.product->getId() == id) {
                item.quantity += qty;
                found = true;
                break;
            }
        }
        if (!found) {
            cart.push_back({p, qty});
        }
        std::cout << "已將 " << qty << " 件 " << p->getName() << " 加入購物車。\n\n";
    }

    if (cart.empty()) {
        std::cout << "購物車為空，結帳取消。\n";
        return;
    }

    // Display receipt
    std::cout << "\n======================================================================\n";
    std::cout << "                         收 銀 結 帳 單                               \n";
    std::cout << "======================================================================\n";
    std::cout << std::left 
              << std::setw(12) << "類型"
              << std::setw(18) << "品名"
              << std::setw(10) << "數量"
              << std::setw(12) << "單價(折後)"
              << "小計\n";
    std::cout << "----------------------------------------------------------------------\n";

    double totalAmount = 0.0;
    for (const auto& item : cart) {
        double subtotal = item.product->getDiscountedPrice() * item.quantity;
        totalAmount += subtotal;
        std::cout << std::left 
                  << std::setw(12) << item.product->getProductType()
                  << std::setw(18) << item.product->getName()
                  << std::setw(10) << item.quantity
                  << std::setw(12) << item.product->getDiscountedPrice()
                  << subtotal << "\n";
    }
    std::cout << "======================================================================\n";
    std::cout << std::right << std::setw(50) << "應付總額: NT$ " << totalAmount << "\n";
    std::cout << "======================================================================\n";

    std::cout << "確認結帳並更新庫存？ (y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        for (const auto& item : cart) {
            item.product->setStock(item.product->getStock() - item.quantity);
        }
        std::cout << "[系統訊息] 結帳成功！庫存已扣除。感謝您的消費！\n";
    } else {
        std::cout << "[系統訊息] 結帳取消，未扣除庫存。\n";
    }
}

int main() {
    InventoryManager manager;
    if (!manager.loadFromFile(FILENAME)) {
        std::cerr << "載入庫存檔案失敗！\n";
        return 1;
    }
    std::cout << "[系統訊息] 庫存資料已載入 (共讀取 " << manager.getProducts().size() << " 筆商品)。\n";

    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "        動態商店收銀與庫存管理系統 (v1.0)         \n";
        std::cout << "==================================================\n";
        std::cout << " 1. 顯示所有商品庫存 (List Inventory)\n";
        std::cout << " 2. 新增商品入庫 (Add Product)\n";
        std::cout << " 3. 刪除商品 (Delete Product)\n";
        std::cout << " 4. 模擬收銀結帳 (POS Checkout Simulation)\n";
        std::cout << " 5. 排序商品清單 (Sort Products)\n";
        std::cout << " 6. 儲存變更至檔案 (Save Data)\n";
        std::cout << " 7. 結束程式 (Exit)\n";
        std::cout << "==================================================\n";
        int choice = getIntegerInput("請輸入您的選擇 (1-7): ");

        switch (choice) {
            case 1:
                printInventory(manager);
                break;
            case 2:
                addProductUI(manager);
                break;
            case 3:
                deleteProductUI(manager);
                break;
            case 4:
                posCheckoutUI(manager);
                break;
            case 5:
                sortProductsUI(manager);
                break;
            case 6:
                if (manager.saveToFile(FILENAME)) {
                    std::cout << "\n[系統訊息] 庫存變更已成功寫入 " << FILENAME << "！\n";
                } else {
                    std::cout << "\n[系統訊息] 儲存檔案時發生錯誤。\n";
                }
                break;
            case 7: {
                std::cout << "結束前是否儲存變更？ (y/n): ";
                char saveChoice;
                std::cin >> saveChoice;
                if (saveChoice == 'y' || saveChoice == 'Y') {
                    if (manager.saveToFile(FILENAME)) {
                        std::cout << "[系統訊息] 資料已儲存。";
                    } else {
                        std::cout << "[系統訊息] 儲存失敗。";
                    }
                }
                std::cout << "\n感謝您使用本系統，再見！\n";
                return 0;
            }
            default:
                std::cout << "無效的選項，請輸入 1 至 7 之間的數字。\n";
                break;
        }
    }
    return 0;
}
