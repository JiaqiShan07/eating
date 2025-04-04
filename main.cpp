#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "DishSelector.h"

// 函数声明
void showMainMenu(DishSelector& selector);
void addNewDish();
void showHelpInfo();
void pauseAndClearConsole(int getchar_num = 1);

int main() {
    DishSelector selector;
    std::string filename =
        "dishes.txt";  // 使用相对路径，默认为当前目录下的dishes.txt

    std::cout << "欢迎使用菜品随机选择器！\n";

    if (!selector.loadDishesFromFile(filename)) {
        std::cout << "无法加载菜品文件！\n";
        return 1;
    }

    std::cout << "成功加载 " << selector.getDishCount() << " 个菜品\n";

    // 显示主菜单
    showMainMenu(selector);

    std::cout << "感谢使用，再见！\n";
    return 0;
}

// 显示主菜单
void showMainMenu(DishSelector& selector) {
    int choice;
    do {
        std::cout << "\n========== 菜品选择器主菜单 ==========\n";
        std::cout << "1. 随机选择菜品\n";
        std::cout << "2. 重新选择菜品\n";
        std::cout << "3. 查看所有菜品\n";
        std::cout << "4. 删除指定菜品\n";
        std::cout << "5. 添加新菜品\n";
        std::cout << "6. 查看帮助信息\n";
        std::cout << "0. 退出程序\n";
        std::cout << "请输入选项 (0-6): ";

        // 防止输入非数字
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请重新输入数字选项\n";
            choice = -1;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {  // 随机选择菜品
                // 获取用户筛选偏好
                bool inSchoolOnly = false;
                bool spicyPreference = false;
                char filterChoice;

                std::cout << "\n----- 菜品属性选择 -----\n";
                std::cout << "是否只选择校内菜品？\n";
                std::cout << "  y - 是，只选择校内菜品\n";
                std::cout << "  n - 否，校内校外都可以\n";
                std::cout << "请选择 (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                inSchoolOnly = (filterChoice == 'y' || filterChoice == 'Y');

                std::cout << "\n是否偏好辣味菜品？\n";
                std::cout << "  y - 是，偏好辣味菜品\n";
                std::cout << "  n - 否，偏好不辣菜品\n";
                std::cout << "请选择 (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                spicyPreference = (filterChoice == 'y' || filterChoice == 'Y');

                try {
                    DishSelector::Dish selectedDish = selector.selectRandomDish(
                        inSchoolOnly, spicyPreference);
                    std::cout << "\n今天推荐的菜品是：" << selectedDish.name;
                    std::cout << "\n菜品属性："
                              << (selectedDish.isInSchool ? "校内" : "校外")
                              << ", " << (selectedDish.isSpicy ? "辣" : "不辣")
                              << "\n";
                } catch (const std::exception& e) {
                    std::cout << "错误：" << e.what() << "\n";
                }
                pauseAndClearConsole();
                break;
            }
            case 2: {  // 重新选择菜品
                // 获取用户筛选偏好
                bool inSchoolOnly = false;
                bool spicyPreference = false;
                char filterChoice;

                std::cout << "\n----- 菜品属性选择 -----\n";
                std::cout << "是否只选择校内菜品？\n";
                std::cout << "  y - 是，只选择校内菜品\n";
                std::cout << "  n - 否，校内校外都可以\n";
                std::cout << "请选择 (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                inSchoolOnly = (filterChoice == 'y' || filterChoice == 'Y');

                std::cout << "\n是否偏好辣味菜品？\n";
                std::cout << "  y - 是，偏好辣味菜品\n";
                std::cout << "  n - 否，偏好不辣菜品\n";
                std::cout << "请选择 (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                spicyPreference = (filterChoice == 'y' || filterChoice == 'Y');

                try {
                    DishSelector::Dish selectedDish =
                        selector.reselectDish(inSchoolOnly, spicyPreference);
                    std::cout << "\n重新选择的菜品是：" << selectedDish.name;
                    std::cout << "\n菜品属性："
                              << (selectedDish.isInSchool ? "校内" : "校外")
                              << ", " << (selectedDish.isSpicy ? "辣" : "不辣")
                              << "\n";
                } catch (const std::exception& e) {
                    std::cout << "错误：" << e.what() << "\n";
                }
                pauseAndClearConsole();
                break;
            }
            case 3: {  // 查看所有菜品
                const auto& allDishes = selector.getAllDishes();
                if (allDishes.empty()) {
                    std::cout << "\n当前没有菜品\n";
                } else {
                    std::cout << "\n----- 所有菜品列表 -----\n";
                    std::cout
                        << "序号 | 菜品名称 | 位置 | 辣度 | 两天内是否食用\n";
                    std::cout << "----------------------------------\n";
                    int index = 1;
                    for (const auto& dish : allDishes) {
                        std::cout << index++ << ". " << dish.name << " | "
                                  << (dish.isInSchool ? "校内" : "校外")
                                  << " | " << (dish.isSpicy ? "辣" : "不辣")
                                  << " | " << (dish.eatenRecently ? "是" : "否")
                                  << "\n";
                    }
                }
                pauseAndClearConsole();
                break;
            }
            case 4: {  // 删除指定菜品
                const auto& allDishes = selector.getAllDishes();
                if (allDishes.empty()) {
                    std::cout << "\n当前没有菜品可删除\n";
                } else {
                    std::cout << "\n----- 删除菜品 -----\n";
                    std::cout << "序号 | 菜品名称 | 位置 | 辣度\n";
                    std::cout << "---------------------------\n";
                    int index = 1;
                    for (const auto& dish : allDishes) {
                        std::cout << index++ << ". " << dish.name << " | "
                                  << (dish.isInSchool ? "校内" : "校外")
                                  << " | " << (dish.isSpicy ? "辣" : "不辣")
                                  << "\n";
                    }

                    int dishIndex;
                    std::cout << "\n请输入要删除的菜品序号 (0取消): ";
                    if (!(std::cin >> dishIndex)) {
                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "输入无效\n";
                    } else if (dishIndex == 0) {
                        std::cout << "取消删除\n";
                    } else if (dishIndex < 1 ||
                               dishIndex > static_cast<int>(allDishes.size())) {
                        std::cout << "无效的序号\n";
                    } else {
                        std::string dishName = allDishes[dishIndex - 1].name;
                        if (selector.deleteDish(dishName)) {
                            std::cout << "成功删除菜品：" << dishName << "\n";
                        } else {
                            std::cout << "删除失败\n";
                        }
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                    '\n');
                }
                pauseAndClearConsole();
                break;
            }
            case 5:
                addNewDish();
                // 重新加载菜品文件
                selector.loadDishesFromFile("dishes.txt");
                pauseAndClearConsole();
                break;
            case 6:
                showHelpInfo();
                pauseAndClearConsole(
                    0);  // 已经在showHelpInfo中有暂停，不需要再次暂停
                break;
            case 0:
                std::cout << "正在退出程序...\n";
                break;
            default:
                std::cout << "无效选项，请重新输入\n";
        }
    } while (choice != 0);
}

// 添加新菜品
void addNewDish() {
    std::string dishName;
    char inSchoolChoice, spicyChoice;
    std::string inSchoolValue, spicyValue;

    std::cout << "\n----- 添加新菜品 -----\n";
    std::cout << "请输入菜品名称: ";
    std::getline(std::cin, dishName);

    if (dishName.empty()) {
        std::cout << "菜品名称不能为空，添加取消\n";
        return;
    }

    std::cout << "该菜品是否在校内可获得？\n";
    std::cout << "  y - 是，校内可获得\n";
    std::cout << "  n - 否，校外菜品\n";
    std::cout << "请选择 (y/n): ";
    std::cin >> inSchoolChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    inSchoolValue =
        (inSchoolChoice == 'y' || inSchoolChoice == 'Y') ? "1" : "0";

    std::cout << "该菜品是否为辣味？\n";
    std::cout << "  y - 是，辣味菜品\n";
    std::cout << "  n - 否，不辣菜品\n";
    std::cout << "请选择 (y/n): ";
    std::cin >> spicyChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    spicyValue = (spicyChoice == 'y' || spicyChoice == 'Y') ? "1" : "0";

    // 格式化菜品信息并添加到文件
    std::string dishInfo = dishName + "|" + inSchoolValue + "|" + spicyValue;

    std::ofstream file("dishes.txt", std::ios::app);
    if (file.is_open()) {
        file << dishInfo << std::endl;
        file.close();
        std::cout << "成功添加菜品：" << dishName << " ("
                  << (inSchoolValue == "1" ? "校内" : "校外") << ", "
                  << (spicyValue == "1" ? "辣" : "不辣") << ")\n";
    } else {
        std::cout << "无法打开菜品文件，添加失败\n";
    }

    pauseAndClearConsole();
}

// 暂停并清屏函数
void pauseAndClearConsole(int getchar_num) {
    if (getchar_num != 0) {
        printf("按Enter键继续...");
        for (int i = 0; i < getchar_num; i++) {
            while (getchar() != '\n') {
                // 清空输入缓冲区，直到遇到换行符
                // 防止用户乱七八糟的输入导致缓冲区溢出，导致后续输入错误
            }
        }
    }
    system("cls");
}

// 显示帮助信息
void showHelpInfo() {
    std::cout << "\n----- 菜品选择器使用帮助 -----\n";
    std::cout << "1. 菜品文件格式:\n";
    std::cout
        << "   每行一个菜品，格式为: 菜品名称|是否校内(1/0)|是否辣(1/0)\n";
    std::cout << "   例如: 红烧肉|1|0 表示红烧肉是校内菜品且不辣\n";
    std::cout << "\n2. 菜品属性说明:\n";
    std::cout << "   - 是否校内: 1表示校内可获得，0表示校外菜品\n";
    std::cout << "   - 是否辣: 1表示辣味菜品，0表示不辣菜品\n";
    std::cout << "   - 两天内是否食用: 根据历史记录自动更新\n";
    std::cout << "\n3. 随机选择菜品:\n";
    std::cout << "   - 可以根据菜品属性（是否校内、是否辣）进行筛选\n";
    std::cout << "   - 程序会确保两天内不会重复推荐同一道菜\n";
    std::cout << "\n4. 重新选择菜品:\n";
    std::cout << "   - 不会选择上一次选择的菜品\n";
    std::cout << "   - 上一次选择的菜品会被标记为两天内食用过\n";
    std::cout << "\n5. 查看所有菜品:\n";
    std::cout << "   - 显示所有菜品及其属性\n";
    std::cout << "   - 包括两天内是否食用的信息\n";
    std::cout << "\n6. 删除指定菜品:\n";
    std::cout << "   - 可以从菜品列表中删除不需要的菜品\n";
    std::cout << "\n7. 添加新菜品:\n";
    std::cout << "   - 通过菜单选项可以添加新的菜品到dishes.txt文件\n";
    std::cout << "\n8. 历史记录格式:\n";
    std::cout << "   - 历史记录包含日期、时间、星期几和菜品属性信息\n";
    std::cout << "   - 格式为: 时间戳|菜品名称|是否校内|是否辣|人类可读时间\n";
    std::cout << "\n按Enter键返回主菜单...";
    std::cin.get();
}