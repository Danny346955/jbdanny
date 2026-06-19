# C++ 期末專題：動態商店收銀與庫存管理系統 (Dynamic Retail POS & Inventory Management System)

本專題是一個基於 C++17 標準實作的商店庫存與收銀管理系統（POS System）。系統透過純文字終端機介面進行操作，完整實現了物件導向設計（Inheritance & Polymorphism）、檔案持久化（File I/O）以及 C++ 標準函式庫（STL）之應用。

---

## 專題核心特色 (期末要求對照)

1. **類別繼承與執行期多型 (Inheritance & Polymorphism)**
   * 設計抽象基底類別 `Product`，並衍生出 `FoodProduct`（食品）、`ClothingProduct`（服飾）與 `ElectronicsProduct`（電子產品）。
   * 利用虛擬解構子確保記憶體正確釋放，並透過覆寫（Override）虛擬函式 `getDiscountedPrice()` 與 `getProductType()` 實作各自的折扣計價邏輯，呈現多型特性。

2. **檔案讀取與寫入 (File I/O)**
   * 系統啟動時，自動讀取同目錄下的 `inventory.txt` 檔案，逐行解析並建立對應的衍生類別商品物件。
   * 系統運行期間的新增、刪除商品與收銀扣除庫存，均可透過選單儲存至 `inventory.txt`，確保資料持久化。

3. **使用 STL 類別庫 (Standard Template Library)**
   * 使用 `std::vector<std::unique_ptr<Product>>` 作為商品容器，利用 `unique_ptr` 進行多型物件的生命週期與記憶體安全管理。
   * 運用 `<algorithm>` 中的 `std::sort` 配合 Lambda 運算式，提供多維度的商品排序功能（如依 ID、依折實售價、依庫存量）。
   * 運用 `<sstream>` 的 `std::stringstream` 實作資料的序列化與反序列化。

4. **終端機 UI 操作介面 (Terminal UI)**
   * 全互動式的終端機操作選單，具備完善的輸入防呆機制（如處理非數字的字元輸入，避免無窮迴圈）。
   * 使用表格化對齊輸出（透過 `<iomanip>` 的 `std::setw` 與 `std::left`），呈現美觀的庫存清單與收銀明細。

---

## 系統架構設計

### 1. 類別架構 (Class Hierarchy)
* **`Product` (Abstract Base Class)**：基本屬性包括 ID、商品名稱（Name）、原價（Base Price）、庫存量（Stock）。
* **`FoodProduct` (食品類)**：
  * 額外屬性：有效天數（Expiry Days）。
  * 促銷折扣邏輯：有效天數 $\le 3$ 天打 5 折；$\le 7$ 天打 8 折；其餘原價。
* **`ClothingProduct` (服飾類)**：
  * 額外屬性：尺寸（Size）、材質（Material）。
  * 促銷折扣邏輯：季末特惠，一律打 9 折。
* **`ElectronicsProduct` (電子產品類)**：
  * 額外屬性：保固月數（Warranty Months）。
  * 促銷折扣邏輯：電子產品無折扣，且需額外加收 5% 的保固服務與環保處理費。

### 2. 檔案序列化格式 (File Format)
`inventory.txt` 的資料格式以逗號分隔（CSV 格式）：
* 食品類：`Food,ID,Name,BasePrice,Stock,ExpiryDays`
* 服飾類：`Clothing,ID,Name,BasePrice,Stock,Size,Material`
* 電子產品：`Electronics,ID,Name,BasePrice,Stock,WarrantyMonths`

---

## 檔案清單

* [Product.h](file:///c:/Users/TUF%20Gaming/Desktop/project/Product.h)：`Product` 抽象基底類別定義。
* [FoodProduct.h](file:///c:/Users/TUF%20Gaming/Desktop/project/FoodProduct.h)：`FoodProduct` 衍生類別定義與計價邏輯。
* [ClothingProduct.h](file:///c:/Users/TUF%20Gaming/Desktop/project/ClothingProduct.h)：`ClothingProduct` 衍生類別定義與折扣邏輯。
* [ElectronicsProduct.h](file:///c:/Users/TUF%20Gaming/Desktop/project/ElectronicsProduct.h)：`ElectronicsProduct` 衍生類別定義與保固費邏輯。
* [InventoryManager.h](file:///c:/Users/TUF%20Gaming/Desktop/project/InventoryManager.h)：`InventoryManager` 庫存管理器類別宣告。
* [InventoryManager.cpp](file:///c:/Users/TUF%20Gaming/Desktop/project/InventoryManager.cpp)：`InventoryManager` 功能實作（檔案讀寫、排序、增刪商品）。
* [main.cpp](file:///c:/Users/TUF%20Gaming/Desktop/project/main.cpp)：主程式，包含終端機互動選單、收銀購物車結帳邏輯與使用者防呆機制。
* [inventory.txt](file:///c:/Users/TUF%20Gaming/Desktop/project/inventory.txt)：商品庫存測試資料庫。

---

## 編譯與執行指南

本專案採用標準 C++ 撰寫，無額外第三方套件依賴，可在任何支援 C++17 或更新版本的編譯器上編譯。

### 方法一：使用 g++ 終端機編譯 (Windows / Command Line)
請開啟終端機（PowerShell 或 CMD）至專案目錄，輸入以下指令：
```bash
g++ -std=c++17 main.cpp InventoryManager.cpp -o store_system.exe
```
編譯完成後，執行程式：
```bash
.\store_system.exe
```

### 方法二：使用 Dev-C++ 編譯與執行
1. 開啟 Dev-C++，點選 `檔案` -> `開新檔案` -> `專案 (Project)`。
2. 選擇 `Console Application`，將專案命名為 `StoreSystem`，並將程式語言設為 `C++`。
3. 將此專案資料夾內的所有原始碼檔案 (`.h` 與 `.cpp`) 加入該專案中。
4. **重要設定**：點選 `專案` -> `專案選項` -> `編譯器 (Compiler)`，在「程式碼產生 (Code Generation)」中的「語言標準 (Language Standard)」選擇 **`ISO C++17`** 或更新版本。
5. 按下 `F11` (編譯並執行) 即可啟動系統。

### 方法三：使用 Visual Studio 編譯與執行
1. 開啟 Visual Studio，點選 `建立新專案` -> 選擇 `C++ 主控台應用程式`。
2. 將所有原始碼檔案拖曳或匯入至專案的「標頭檔」與「來源檔案」中。
3. 將 `inventory.txt` 放入專案的輸出目錄（或與產生的 `.exe` 相同目錄下）。
4. 點選 `建置` -> `建置方案`，然後按下 `Ctrl + F5` 執行。

---

## 終端機介面操作展示

### 1. 主選單畫面
```text
==================================================
        動態商店收銀與庫存管理系統 (v1.0)
==================================================
 1. 顯示所有商品庫存 (List Inventory)
 2. 新增商品入庫 (Add Product)
 3. 刪除商品 (Delete Product)
 4. 模擬收銀結帳 (POS Checkout Simulation)
 5. 排序商品清單 (Sort Products)
 6. 儲存變更至檔案 (Save Data)
 7. 結束程式 (Exit)
==================================================
請輸入您的選擇 (1-7): 
```

### 2. 顯示商品清單 (按折實售價升序排序後)
```text
------------------------------------------------------------------------------------------
商品類型    ID      名稱               原價        折實/計費   庫存    詳細屬性
------------------------------------------------------------------------------------------
Food        101     Fresh_Milk         80          40          15      Expiry: 3 days
Food        102     Apple_Bag          120         96          20      Expiry: 6 days
Food        103     Potato_Chips       60          60          40      Expiry: 180 days
Clothing    201     Hooded_Sweater     850         765         15      Size: M, Material: Cotton
Clothing    202     Denim_Jeans        1200        1080        10      Size: L, Material: Denim
Electronics 301     Gaming_Mouse       1500        1575        12      Warranty: 12 months
Electronics 302     Wireless_Keyboard  2200        2310        8       Warranty: 24 months
------------------------------------------------------------------------------------------
```

### 3. POS 收銀結帳模擬畫面
```text
=============================================
         模擬收銀結帳系統 (POS Simulator)    
=============================================
輸入要購買的商品 ID (輸入 0 結束選購): 101
選中商品: Fresh_Milk (類型: Food, 目前庫存: 15, 最終單價: 40)
請輸入購買數量: 2
已將 2 件 Fresh_Milk 加入購物車。

輸入要購買的商品 ID (輸入 0 結束選購): 201
選中商品: Hooded_Sweater (類型: Clothing, 目前庫存: 15, 最終單價: 765)
請輸入購買數量: 1
已將 1 件 Hooded_Sweater 加入購物車。

輸入要購買的商品 ID (輸入 0 結束選購): 0

======================================================================
                         收 銀 結 帳 單                               
======================================================================
類型        品名               數量      單價(折後)  小計
----------------------------------------------------------------------
Food        Fresh_Milk         2         40          80
Clothing    Hooded_Sweater     1         765         765
======================================================================
                                              應付總額: NT$ 845
======================================================================
確認結帳並更新庫存？ (y/n): y
[系統訊息] 結帳成功！庫存已扣除。感謝您的消費！
```
