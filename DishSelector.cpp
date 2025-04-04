#include "DishSelector.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

DishSelector::DishSelector()
    : gen(static_cast<unsigned int>(std::time(nullptr))) {
    // 使用当前时间作为种子初始化随机数生成器，增强随机性
}

bool DishSelector::loadDishesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    dishes.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            std::istringstream iss(line);
            std::string dishName;
            std::string inSchoolStr;
            std::string spicyStr;

            // 格式：菜品名称|是否校内|是否辣
            if (std::getline(iss, dishName, '|') &&
                std::getline(iss, inSchoolStr, '|') &&
                std::getline(iss, spicyStr)) {
                Dish dish;
                dish.name = dishName;
                dish.isInSchool = (inSchoolStr == "1" || inSchoolStr == "true");
                dish.isSpicy = (spicyStr == "1" || spicyStr == "true");
                dishes.push_back(dish);
            } else {
                // 兼容旧格式，默认为校内且不辣
                Dish dish;
                dish.name = line;
                dish.isInSchool = true;
                dish.isSpicy = false;
                dishes.push_back(dish);
            }
        }
    }
    file.close();
    return !dishes.empty();
}

bool DishSelector::loadHistory() {
    std::ifstream file(historyFile);
    if (!file.is_open()) {
        return false;
    }

    history.clear();
    std::string line;
    std::time_t now = std::time(nullptr);

    // 获取当前日期
    std::tm* now_tm = std::localtime(&now);
    int now_day = now_tm->tm_mday;
    int now_month = now_tm->tm_mon;
    int now_year = now_tm->tm_year;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string dishName, timestamp_str, inSchoolStr, spicyStr;

        if (std::getline(iss, timestamp_str, '|') &&
            std::getline(iss, dishName, '|') &&
            std::getline(iss, inSchoolStr, '|') &&
            std::getline(iss, spicyStr)) {
            DishHistory entry;
            entry.dishName = dishName;
            entry.timestamp = std::stoll(timestamp_str);
            entry.isInSchool = (inSchoolStr == "1" || inSchoolStr == "true");
            entry.isSpicy = (spicyStr == "1" || spicyStr == "true");
            history.push_back(entry);

            // 更新菜品的eatenRecently属性
            for (auto& dish : dishes) {
                if (dish.name == dishName) {
                    // 获取历史记录的日期
                    std::tm* entry_tm = std::localtime(&entry.timestamp);
                    int entry_day = entry_tm->tm_mday;
                    int entry_month = entry_tm->tm_mon;
                    int entry_year = entry_tm->tm_year;

                    // 计算日期差
                    bool isRecent = false;

                    // 如果是同一年同一月，直接比较日期差
                    if (now_year == entry_year && now_month == entry_month) {
                        isRecent = (now_day - entry_day < 2);
                    }
                    // 如果是同一年但不同月
                    else if (now_year == entry_year &&
                             now_month - entry_month == 1) {
                        // 获取上个月的天数
                        int days_in_entry_month;
                        if (entry_month == 1) {  // 2月
                            // 检查闰年
                            bool is_leap_year = (entry_year % 4 == 0 &&
                                                 entry_year % 100 != 0) ||
                                                (entry_year % 400 == 0);
                            days_in_entry_month = is_leap_year ? 29 : 28;
                        } else if (entry_month == 3 || entry_month == 5 ||
                                   entry_month == 8 || entry_month == 10) {
                            days_in_entry_month = 30;
                        } else {
                            days_in_entry_month = 31;
                        }

                        // 如果日期差小于2天
                        isRecent =
                            ((days_in_entry_month - entry_day) + now_day < 2);
                    }
                    // 不考虑跨年的情况，默认跨年已经超过两天

                    dish.eatenRecently = isRecent;
                    break;
                }
            }
        } else if (std::getline(iss, timestamp_str, '|') &&
                   std::getline(iss, dishName)) {
            // 兼容旧格式
            DishHistory entry;
            entry.dishName = dishName;
            entry.timestamp = std::stoll(timestamp_str);
            entry.isInSchool = true;  // 默认值
            entry.isSpicy = false;    // 默认值
            history.push_back(entry);

            // 更新菜品的eatenRecently属性
            for (auto& dish : dishes) {
                if (dish.name == dishName) {
                    // 获取历史记录的日期
                    std::tm* entry_tm = std::localtime(&entry.timestamp);
                    int entry_day = entry_tm->tm_mday;
                    int entry_month = entry_tm->tm_mon;
                    int entry_year = entry_tm->tm_year;

                    // 计算日期差
                    bool isRecent = false;

                    // 如果是同一年同一月，直接比较日期差
                    if (now_year == entry_year && now_month == entry_month) {
                        isRecent = (now_day - entry_day < 2);
                    }
                    // 如果是同一年但不同月
                    else if (now_year == entry_year &&
                             now_month - entry_month == 1) {
                        // 获取上个月的天数
                        int days_in_entry_month;
                        if (entry_month == 1) {  // 2月
                            // 检查闰年
                            bool is_leap_year = (entry_year % 4 == 0 &&
                                                 entry_year % 100 != 0) ||
                                                (entry_year % 400 == 0);
                            days_in_entry_month = is_leap_year ? 29 : 28;
                        } else if (entry_month == 3 || entry_month == 5 ||
                                   entry_month == 8 || entry_month == 10) {
                            days_in_entry_month = 30;
                        } else {
                            days_in_entry_month = 31;
                        }

                        // 如果日期差小于2天
                        isRecent =
                            ((days_in_entry_month - entry_day) + now_day < 2);
                    }
                    // 不考虑跨年的情况，默认跨年已经超过两天

                    dish.eatenRecently = isRecent;
                    break;
                }
            }
        }
    }
    file.close();
    return true;
}

void DishSelector::saveHistory(const std::string& dishName) {
    // 查找菜品属性
    bool isInSchool = true;
    bool isSpicy = false;
    for (const auto& dish : dishes) {
        if (dish.name == dishName) {
            isInSchool = dish.isInSchool;
            isSpicy = dish.isSpicy;
            break;
        }
    }

    std::ofstream file(historyFile, std::ios::app);
    if (file.is_open()) {
        std::time_t now = std::time(nullptr);

        // 格式化时间为可读格式
        std::tm* timeinfo = std::localtime(&now);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %A", timeinfo);

        // 保存格式：时间戳|菜品名称|是否校内|是否辣
        file << now << "|" << dishName << "|" << (isInSchool ? "1" : "0") << "|"
             << (isSpicy ? "1" : "0") << "|" << buffer << std::endl;
        file.close();

        // 更新内存中的历史记录
        DishHistory entry;
        entry.dishName = dishName;
        entry.timestamp = now;
        entry.isInSchool = isInSchool;
        entry.isSpicy = isSpicy;
        history.push_back(entry);

        // 更新菜品的eatenRecently属性
        for (auto& dish : dishes) {
            if (dish.name == dishName) {
                dish.eatenRecently = true;
                break;
            }
        }
    }
}

bool DishSelector::isDishSelectedRecently(const std::string& dishName) const {
    std::time_t now = std::time(nullptr);

    // 获取当前日期
    std::tm* now_tm = std::localtime(&now);
    int now_day = now_tm->tm_mday;
    int now_month = now_tm->tm_mon;
    int now_year = now_tm->tm_year;

    for (const auto& entry : history) {
        if (entry.dishName == dishName) {
            // 获取历史记录的日期
            std::tm* entry_tm = std::localtime(&entry.timestamp);
            int entry_day = entry_tm->tm_mday;
            int entry_month = entry_tm->tm_mon;
            int entry_year = entry_tm->tm_year;

            // 计算日期差
            // 如果是同一年同一月，直接比较日期差
            if (now_year == entry_year && now_month == entry_month) {
                if (now_day - entry_day < 2) {
                    return true;
                }
            }
            // 如果是同一年但不同月
            else if (now_year == entry_year) {
                // 处理月份交界的情况
                if (now_month - entry_month == 1) {
                    // 获取上个月的天数
                    int days_in_entry_month;
                    if (entry_month == 1) {  // 2月
                        // 检查闰年
                        bool is_leap_year =
                            (entry_year % 4 == 0 && entry_year % 100 != 0) ||
                            (entry_year % 400 == 0);
                        days_in_entry_month = is_leap_year ? 29 : 28;
                    } else if (entry_month == 3 || entry_month == 5 ||
                               entry_month == 8 || entry_month == 10) {
                        days_in_entry_month = 30;
                    } else {
                        days_in_entry_month = 31;
                    }

                    // 如果日期差小于2天
                    if ((days_in_entry_month - entry_day) + now_day < 2) {
                        return true;
                    }
                }
            }
            // 不考虑跨年的情况，默认跨年已经超过两天
        }
    }
    return false;
}

DishSelector::Dish DishSelector::selectRandomDish(bool inSchoolOnly,
                                                  bool spicyPreference) {
    if (dishes.empty()) {
        throw std::runtime_error("没有可选择的菜品");
    }

    // 加载历史记录
    loadHistory();

    // 筛选出符合条件且两天内未使用的菜品
    std::vector<Dish> availableDishes;
    for (const auto& dish : dishes) {
        // 根据用户偏好筛选
        if ((inSchoolOnly && !dish.isInSchool) ||
            (spicyPreference && !dish.isSpicy)) {
            continue;
        }

        if (!isDishSelectedRecently(dish.name)) {
            availableDishes.push_back(dish);
        }
    }

    if (availableDishes.empty()) {
        // 如果没有符合条件的菜品，尝试放宽条件
        if (inSchoolOnly || spicyPreference) {
            throw std::runtime_error(
                "没有符合条件的可用菜品，请尝试放宽筛选条件");
        } else {
            throw std::runtime_error(
                "所有菜品都在两天内被选择过，请等待一段时间再试");
        }
    }

    // 随机选择一个可用的菜品
    std::uniform_int_distribution<> dis(0, availableDishes.size() - 1);
    Dish selectedDish = availableDishes[dis(gen)];

    // 保存选择记录
    saveHistory(selectedDish.name);

    return selectedDish;
}

size_t DishSelector::getDishCount() const {
    return dishes.size();
}

const std::vector<DishSelector::Dish>& DishSelector::getAllDishes() const {
    return dishes;
}

bool DishSelector::deleteDish(const std::string& dishName) {
    auto it = std::find_if(
        dishes.begin(), dishes.end(),
        [&dishName](const Dish& dish) { return dish.name == dishName; });

    if (it != dishes.end()) {
        dishes.erase(it);

        // 更新dishes.txt文件
        std::ofstream file("dishes.txt");
        if (file.is_open()) {
            for (const auto& dish : dishes) {
                file << dish.name << "|" << (dish.isInSchool ? "1" : "0") << "|"
                     << (dish.isSpicy ? "1" : "0") << std::endl;
            }
            file.close();
            return true;
        }
    }
    return false;
}

DishSelector::Dish DishSelector::reselectDish(bool inSchoolOnly,
                                              bool spicyPreference) {
    if (dishes.empty()) {
        throw std::runtime_error("没有可选择的菜品");
    }

    // 加载历史记录
    loadHistory();

    // 获取上一次选择的菜品（如果有）
    std::string lastSelectedDish = "";
    if (!history.empty()) {
        lastSelectedDish = history.back().dishName;

        // 将上一次选择的菜品标记为两天内没有食用过
        for (auto& dish : dishes) {
            if (dish.name == lastSelectedDish) {
                dish.eatenRecently = false;
                break;
            }
        }
    }

    // 筛选出符合条件且不是上一次选择的菜品
    std::vector<Dish> availableDishes;
    for (const auto& dish : dishes) {
        // 根据用户偏好筛选
        if ((inSchoolOnly && !dish.isInSchool) ||
            (spicyPreference && !dish.isSpicy)) {
            continue;
        }

        // 排除上一次选择的菜品
        if (dish.name != lastSelectedDish) {
            availableDishes.push_back(dish);
        }
    }

    if (availableDishes.empty()) {
        // 如果没有符合条件的菜品，尝试放宽条件
        if (inSchoolOnly || spicyPreference) {
            throw std::runtime_error(
                "没有符合条件的可用菜品，请尝试放宽筛选条件");
        } else {
            throw std::runtime_error("没有其他可选择的菜品，请添加更多菜品");
        }
    }

    // 随机选择一个可用的菜品
    std::uniform_int_distribution<> dis(0, availableDishes.size() - 1);
    Dish selectedDish = availableDishes[dis(gen)];

    // 保存选择记录
    saveHistory(selectedDish.name);

    return selectedDish;
}