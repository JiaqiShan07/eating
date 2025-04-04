#include "DishSelector.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

DishSelector::DishSelector()
    : gen(static_cast<unsigned int>(std::time(nullptr))) {
    // ʹ�õ�ǰʱ����Ϊ���ӳ�ʼ�����������������ǿ�����
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

            // ��ʽ����Ʒ����|�Ƿ�У��|�Ƿ���
            if (std::getline(iss, dishName, '|') &&
                std::getline(iss, inSchoolStr, '|') &&
                std::getline(iss, spicyStr)) {
                Dish dish;
                dish.name = dishName;
                dish.isInSchool = (inSchoolStr == "1" || inSchoolStr == "true");
                dish.isSpicy = (spicyStr == "1" || spicyStr == "true");
                dishes.push_back(dish);
            } else {
                // ���ݾɸ�ʽ��Ĭ��ΪУ���Ҳ���
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

    // ��ȡ��ǰ����
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

            // ���²�Ʒ��eatenRecently����
            for (auto& dish : dishes) {
                if (dish.name == dishName) {
                    // ��ȡ��ʷ��¼������
                    std::tm* entry_tm = std::localtime(&entry.timestamp);
                    int entry_day = entry_tm->tm_mday;
                    int entry_month = entry_tm->tm_mon;
                    int entry_year = entry_tm->tm_year;

                    // �������ڲ�
                    bool isRecent = false;

                    // �����ͬһ��ͬһ�£�ֱ�ӱȽ����ڲ�
                    if (now_year == entry_year && now_month == entry_month) {
                        isRecent = (now_day - entry_day < 2);
                    }
                    // �����ͬһ�굫��ͬ��
                    else if (now_year == entry_year &&
                             now_month - entry_month == 1) {
                        // ��ȡ�ϸ��µ�����
                        int days_in_entry_month;
                        if (entry_month == 1) {  // 2��
                            // �������
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

                        // ������ڲ�С��2��
                        isRecent =
                            ((days_in_entry_month - entry_day) + now_day < 2);
                    }
                    // �����ǿ���������Ĭ�Ͽ����Ѿ���������

                    dish.eatenRecently = isRecent;
                    break;
                }
            }
        } else if (std::getline(iss, timestamp_str, '|') &&
                   std::getline(iss, dishName)) {
            // ���ݾɸ�ʽ
            DishHistory entry;
            entry.dishName = dishName;
            entry.timestamp = std::stoll(timestamp_str);
            entry.isInSchool = true;  // Ĭ��ֵ
            entry.isSpicy = false;    // Ĭ��ֵ
            history.push_back(entry);

            // ���²�Ʒ��eatenRecently����
            for (auto& dish : dishes) {
                if (dish.name == dishName) {
                    // ��ȡ��ʷ��¼������
                    std::tm* entry_tm = std::localtime(&entry.timestamp);
                    int entry_day = entry_tm->tm_mday;
                    int entry_month = entry_tm->tm_mon;
                    int entry_year = entry_tm->tm_year;

                    // �������ڲ�
                    bool isRecent = false;

                    // �����ͬһ��ͬһ�£�ֱ�ӱȽ����ڲ�
                    if (now_year == entry_year && now_month == entry_month) {
                        isRecent = (now_day - entry_day < 2);
                    }
                    // �����ͬһ�굫��ͬ��
                    else if (now_year == entry_year &&
                             now_month - entry_month == 1) {
                        // ��ȡ�ϸ��µ�����
                        int days_in_entry_month;
                        if (entry_month == 1) {  // 2��
                            // �������
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

                        // ������ڲ�С��2��
                        isRecent =
                            ((days_in_entry_month - entry_day) + now_day < 2);
                    }
                    // �����ǿ���������Ĭ�Ͽ����Ѿ���������

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
    // ���Ҳ�Ʒ����
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

        // ��ʽ��ʱ��Ϊ�ɶ���ʽ
        std::tm* timeinfo = std::localtime(&now);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %A", timeinfo);

        // �����ʽ��ʱ���|��Ʒ����|�Ƿ�У��|�Ƿ���
        file << now << "|" << dishName << "|" << (isInSchool ? "1" : "0") << "|"
             << (isSpicy ? "1" : "0") << "|" << buffer << std::endl;
        file.close();

        // �����ڴ��е���ʷ��¼
        DishHistory entry;
        entry.dishName = dishName;
        entry.timestamp = now;
        entry.isInSchool = isInSchool;
        entry.isSpicy = isSpicy;
        history.push_back(entry);

        // ���²�Ʒ��eatenRecently����
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

    // ��ȡ��ǰ����
    std::tm* now_tm = std::localtime(&now);
    int now_day = now_tm->tm_mday;
    int now_month = now_tm->tm_mon;
    int now_year = now_tm->tm_year;

    for (const auto& entry : history) {
        if (entry.dishName == dishName) {
            // ��ȡ��ʷ��¼������
            std::tm* entry_tm = std::localtime(&entry.timestamp);
            int entry_day = entry_tm->tm_mday;
            int entry_month = entry_tm->tm_mon;
            int entry_year = entry_tm->tm_year;

            // �������ڲ�
            // �����ͬһ��ͬһ�£�ֱ�ӱȽ����ڲ�
            if (now_year == entry_year && now_month == entry_month) {
                if (now_day - entry_day < 2) {
                    return true;
                }
            }
            // �����ͬһ�굫��ͬ��
            else if (now_year == entry_year) {
                // �����·ݽ�������
                if (now_month - entry_month == 1) {
                    // ��ȡ�ϸ��µ�����
                    int days_in_entry_month;
                    if (entry_month == 1) {  // 2��
                        // �������
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

                    // ������ڲ�С��2��
                    if ((days_in_entry_month - entry_day) + now_day < 2) {
                        return true;
                    }
                }
            }
            // �����ǿ���������Ĭ�Ͽ����Ѿ���������
        }
    }
    return false;
}

DishSelector::Dish DishSelector::selectRandomDish(bool inSchoolOnly,
                                                  bool spicyPreference) {
    if (dishes.empty()) {
        throw std::runtime_error("û�п�ѡ��Ĳ�Ʒ");
    }

    // ������ʷ��¼
    loadHistory();

    // ɸѡ������������������δʹ�õĲ�Ʒ
    std::vector<Dish> availableDishes;
    for (const auto& dish : dishes) {
        // �����û�ƫ��ɸѡ
        if ((inSchoolOnly && !dish.isInSchool) ||
            (spicyPreference && !dish.isSpicy)) {
            continue;
        }

        if (!isDishSelectedRecently(dish.name)) {
            availableDishes.push_back(dish);
        }
    }

    if (availableDishes.empty()) {
        // ���û�з��������Ĳ�Ʒ�����Էſ�����
        if (inSchoolOnly || spicyPreference) {
            throw std::runtime_error(
                "û�з��������Ŀ��ò�Ʒ���볢�Էſ�ɸѡ����");
        } else {
            throw std::runtime_error(
                "���в�Ʒ���������ڱ�ѡ�������ȴ�һ��ʱ������");
        }
    }

    // ���ѡ��һ�����õĲ�Ʒ
    std::uniform_int_distribution<> dis(0, availableDishes.size() - 1);
    Dish selectedDish = availableDishes[dis(gen)];

    // ����ѡ���¼
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

        // ����dishes.txt�ļ�
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
        throw std::runtime_error("û�п�ѡ��Ĳ�Ʒ");
    }

    // ������ʷ��¼
    loadHistory();

    // ��ȡ��һ��ѡ��Ĳ�Ʒ������У�
    std::string lastSelectedDish = "";
    if (!history.empty()) {
        lastSelectedDish = history.back().dishName;

        // ����һ��ѡ��Ĳ�Ʒ���Ϊ������û��ʳ�ù�
        for (auto& dish : dishes) {
            if (dish.name == lastSelectedDish) {
                dish.eatenRecently = false;
                break;
            }
        }
    }

    // ɸѡ�����������Ҳ�����һ��ѡ��Ĳ�Ʒ
    std::vector<Dish> availableDishes;
    for (const auto& dish : dishes) {
        // �����û�ƫ��ɸѡ
        if ((inSchoolOnly && !dish.isInSchool) ||
            (spicyPreference && !dish.isSpicy)) {
            continue;
        }

        // �ų���һ��ѡ��Ĳ�Ʒ
        if (dish.name != lastSelectedDish) {
            availableDishes.push_back(dish);
        }
    }

    if (availableDishes.empty()) {
        // ���û�з��������Ĳ�Ʒ�����Էſ�����
        if (inSchoolOnly || spicyPreference) {
            throw std::runtime_error(
                "û�з��������Ŀ��ò�Ʒ���볢�Էſ�ɸѡ����");
        } else {
            throw std::runtime_error("û��������ѡ��Ĳ�Ʒ������Ӹ����Ʒ");
        }
    }

    // ���ѡ��һ�����õĲ�Ʒ
    std::uniform_int_distribution<> dis(0, availableDishes.size() - 1);
    Dish selectedDish = availableDishes[dis(gen)];

    // ����ѡ���¼
    saveHistory(selectedDish.name);

    return selectedDish;
}