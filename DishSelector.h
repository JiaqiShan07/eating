#ifndef DISH_SELECTOR_H
#define DISH_SELECTOR_H

#include <ctime>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <vector>

class DishSelector {
   public:
    // 菜品结构体，包含名称、是否校内、是否辣、两天内是否食用
    struct Dish {
        std::string name;
        bool isInSchool;
        bool isSpicy;
        bool eatenRecently = false;  // 两天内是否食用
    };

    DishSelector();

    // 从文件加载菜品列表
    bool loadDishesFromFile(const std::string& filename);

    // 随机选择一道菜品，确保两天内不重复
    Dish selectRandomDish(bool inSchoolOnly = false,
                          bool spicyPreference = false);

    // 重新选择菜品，确保不与上一次选择的相同
    Dish reselectDish(bool inSchoolOnly = false, bool spicyPreference = false);

    // 获取所有菜品列表
    const std::vector<Dish>& getAllDishes() const;

    // 删除指定菜品
    bool deleteDish(const std::string& dishName);

    // 获取当前已加载的菜品数量
    size_t getDishCount() const;

   private:
    // 历史记录结构体
    struct DishHistory {
        std::string dishName;
        std::time_t timestamp;
        bool isInSchool;
        bool isSpicy;
    };

    bool loadHistory();                             // 加载历史记录
    void saveHistory(const std::string& dishName);  // 保存历史记录
    bool isDishSelectedRecently(
        const std::string& dishName) const;  // 检查菜品是否在两天内被选择过

    std::vector<Dish> dishes;                       // 存储所有菜品
    std::vector<DishHistory> history;               // 存储历史选择记录
    std::random_device rd;                          // 随机数生成设备
    std::mt19937 gen;                               // 随机数生成器
    const std::string historyFile = "history.txt";  // 历史记录文件路径
};

#endif  // DISH_SELECTOR_H