#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "DishSelector.h"

// ��������
void showMainMenu(DishSelector& selector);
void addNewDish();
void showHelpInfo();
void pauseAndClearConsole(int getchar_num = 1);

int main() {
    DishSelector selector;
    std::string filename =
        "dishes.txt";  // ʹ�����·����Ĭ��Ϊ��ǰĿ¼�µ�dishes.txt

    std::cout << "��ӭʹ�ò�Ʒ���ѡ������\n";

    if (!selector.loadDishesFromFile(filename)) {
        std::cout << "�޷����ز�Ʒ�ļ���\n";
        return 1;
    }

    std::cout << "�ɹ����� " << selector.getDishCount() << " ����Ʒ\n";

    // ��ʾ���˵�
    showMainMenu(selector);

    std::cout << "��лʹ�ã��ټ���\n";
    return 0;
}

// ��ʾ���˵�
void showMainMenu(DishSelector& selector) {
    int choice;
    do {
        std::cout << "\n========== ��Ʒѡ�������˵� ==========\n";
        std::cout << "1. ���ѡ���Ʒ\n";
        std::cout << "2. ����ѡ���Ʒ\n";
        std::cout << "3. �鿴���в�Ʒ\n";
        std::cout << "4. ɾ��ָ����Ʒ\n";
        std::cout << "5. ����²�Ʒ\n";
        std::cout << "6. �鿴������Ϣ\n";
        std::cout << "0. �˳�����\n";
        std::cout << "������ѡ�� (0-6): ";

        // ��ֹ���������
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "������Ч����������������ѡ��\n";
            choice = -1;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {  // ���ѡ���Ʒ
                // ��ȡ�û�ɸѡƫ��
                bool inSchoolOnly = false;
                bool spicyPreference = false;
                char filterChoice;

                std::cout << "\n----- ��Ʒ����ѡ�� -----\n";
                std::cout << "�Ƿ�ֻѡ��У�ڲ�Ʒ��\n";
                std::cout << "  y - �ǣ�ֻѡ��У�ڲ�Ʒ\n";
                std::cout << "  n - ��У��У�ⶼ����\n";
                std::cout << "��ѡ�� (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                inSchoolOnly = (filterChoice == 'y' || filterChoice == 'Y');

                std::cout << "\n�Ƿ�ƫ����ζ��Ʒ��\n";
                std::cout << "  y - �ǣ�ƫ����ζ��Ʒ\n";
                std::cout << "  n - ��ƫ�ò�����Ʒ\n";
                std::cout << "��ѡ�� (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                spicyPreference = (filterChoice == 'y' || filterChoice == 'Y');

                try {
                    DishSelector::Dish selectedDish = selector.selectRandomDish(
                        inSchoolOnly, spicyPreference);
                    std::cout << "\n�����Ƽ��Ĳ�Ʒ�ǣ�" << selectedDish.name;
                    std::cout << "\n��Ʒ���ԣ�"
                              << (selectedDish.isInSchool ? "У��" : "У��")
                              << ", " << (selectedDish.isSpicy ? "��" : "����")
                              << "\n";
                } catch (const std::exception& e) {
                    std::cout << "����" << e.what() << "\n";
                }
                pauseAndClearConsole();
                break;
            }
            case 2: {  // ����ѡ���Ʒ
                // ��ȡ�û�ɸѡƫ��
                bool inSchoolOnly = false;
                bool spicyPreference = false;
                char filterChoice;

                std::cout << "\n----- ��Ʒ����ѡ�� -----\n";
                std::cout << "�Ƿ�ֻѡ��У�ڲ�Ʒ��\n";
                std::cout << "  y - �ǣ�ֻѡ��У�ڲ�Ʒ\n";
                std::cout << "  n - ��У��У�ⶼ����\n";
                std::cout << "��ѡ�� (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                inSchoolOnly = (filterChoice == 'y' || filterChoice == 'Y');

                std::cout << "\n�Ƿ�ƫ����ζ��Ʒ��\n";
                std::cout << "  y - �ǣ�ƫ����ζ��Ʒ\n";
                std::cout << "  n - ��ƫ�ò�����Ʒ\n";
                std::cout << "��ѡ�� (y/n): ";
                std::cin >> filterChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                spicyPreference = (filterChoice == 'y' || filterChoice == 'Y');

                try {
                    DishSelector::Dish selectedDish =
                        selector.reselectDish(inSchoolOnly, spicyPreference);
                    std::cout << "\n����ѡ��Ĳ�Ʒ�ǣ�" << selectedDish.name;
                    std::cout << "\n��Ʒ���ԣ�"
                              << (selectedDish.isInSchool ? "У��" : "У��")
                              << ", " << (selectedDish.isSpicy ? "��" : "����")
                              << "\n";
                } catch (const std::exception& e) {
                    std::cout << "����" << e.what() << "\n";
                }
                pauseAndClearConsole();
                break;
            }
            case 3: {  // �鿴���в�Ʒ
                const auto& allDishes = selector.getAllDishes();
                if (allDishes.empty()) {
                    std::cout << "\n��ǰû�в�Ʒ\n";
                } else {
                    std::cout << "\n----- ���в�Ʒ�б� -----\n";
                    std::cout
                        << "��� | ��Ʒ���� | λ�� | ���� | �������Ƿ�ʳ��\n";
                    std::cout << "----------------------------------\n";
                    int index = 1;
                    for (const auto& dish : allDishes) {
                        std::cout << index++ << ". " << dish.name << " | "
                                  << (dish.isInSchool ? "У��" : "У��")
                                  << " | " << (dish.isSpicy ? "��" : "����")
                                  << " | " << (dish.eatenRecently ? "��" : "��")
                                  << "\n";
                    }
                }
                pauseAndClearConsole();
                break;
            }
            case 4: {  // ɾ��ָ����Ʒ
                const auto& allDishes = selector.getAllDishes();
                if (allDishes.empty()) {
                    std::cout << "\n��ǰû�в�Ʒ��ɾ��\n";
                } else {
                    std::cout << "\n----- ɾ����Ʒ -----\n";
                    std::cout << "��� | ��Ʒ���� | λ�� | ����\n";
                    std::cout << "---------------------------\n";
                    int index = 1;
                    for (const auto& dish : allDishes) {
                        std::cout << index++ << ". " << dish.name << " | "
                                  << (dish.isInSchool ? "У��" : "У��")
                                  << " | " << (dish.isSpicy ? "��" : "����")
                                  << "\n";
                    }

                    int dishIndex;
                    std::cout << "\n������Ҫɾ���Ĳ�Ʒ��� (0ȡ��): ";
                    if (!(std::cin >> dishIndex)) {
                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "������Ч\n";
                    } else if (dishIndex == 0) {
                        std::cout << "ȡ��ɾ��\n";
                    } else if (dishIndex < 1 ||
                               dishIndex > static_cast<int>(allDishes.size())) {
                        std::cout << "��Ч�����\n";
                    } else {
                        std::string dishName = allDishes[dishIndex - 1].name;
                        if (selector.deleteDish(dishName)) {
                            std::cout << "�ɹ�ɾ����Ʒ��" << dishName << "\n";
                        } else {
                            std::cout << "ɾ��ʧ��\n";
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
                // ���¼��ز�Ʒ�ļ�
                selector.loadDishesFromFile("dishes.txt");
                pauseAndClearConsole();
                break;
            case 6:
                showHelpInfo();
                pauseAndClearConsole(
                    0);  // �Ѿ���showHelpInfo������ͣ������Ҫ�ٴ���ͣ
                break;
            case 0:
                std::cout << "�����˳�����...\n";
                break;
            default:
                std::cout << "��Чѡ�����������\n";
        }
    } while (choice != 0);
}

// ����²�Ʒ
void addNewDish() {
    std::string dishName;
    char inSchoolChoice, spicyChoice;
    std::string inSchoolValue, spicyValue;

    std::cout << "\n----- ����²�Ʒ -----\n";
    std::cout << "�������Ʒ����: ";
    std::getline(std::cin, dishName);

    if (dishName.empty()) {
        std::cout << "��Ʒ���Ʋ���Ϊ�գ����ȡ��\n";
        return;
    }

    std::cout << "�ò�Ʒ�Ƿ���У�ڿɻ�ã�\n";
    std::cout << "  y - �ǣ�У�ڿɻ��\n";
    std::cout << "  n - ��У���Ʒ\n";
    std::cout << "��ѡ�� (y/n): ";
    std::cin >> inSchoolChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    inSchoolValue =
        (inSchoolChoice == 'y' || inSchoolChoice == 'Y') ? "1" : "0";

    std::cout << "�ò�Ʒ�Ƿ�Ϊ��ζ��\n";
    std::cout << "  y - �ǣ���ζ��Ʒ\n";
    std::cout << "  n - �񣬲�����Ʒ\n";
    std::cout << "��ѡ�� (y/n): ";
    std::cin >> spicyChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    spicyValue = (spicyChoice == 'y' || spicyChoice == 'Y') ? "1" : "0";

    // ��ʽ����Ʒ��Ϣ����ӵ��ļ�
    std::string dishInfo = dishName + "|" + inSchoolValue + "|" + spicyValue;

    std::ofstream file("dishes.txt", std::ios::app);
    if (file.is_open()) {
        file << dishInfo << std::endl;
        file.close();
        std::cout << "�ɹ���Ӳ�Ʒ��" << dishName << " ("
                  << (inSchoolValue == "1" ? "У��" : "У��") << ", "
                  << (spicyValue == "1" ? "��" : "����") << ")\n";
    } else {
        std::cout << "�޷��򿪲�Ʒ�ļ������ʧ��\n";
    }

    pauseAndClearConsole();
}

// ��ͣ����������
void pauseAndClearConsole(int getchar_num) {
    if (getchar_num != 0) {
        printf("��Enter������...");
        for (int i = 0; i < getchar_num; i++) {
            while (getchar() != '\n') {
                // ������뻺������ֱ���������з�
                // ��ֹ�û����߰�������뵼�»�������������º����������
            }
        }
    }
    system("cls");
}

// ��ʾ������Ϣ
void showHelpInfo() {
    std::cout << "\n----- ��Ʒѡ����ʹ�ð��� -----\n";
    std::cout << "1. ��Ʒ�ļ���ʽ:\n";
    std::cout
        << "   ÿ��һ����Ʒ����ʽΪ: ��Ʒ����|�Ƿ�У��(1/0)|�Ƿ���(1/0)\n";
    std::cout << "   ����: ������|1|0 ��ʾ��������У�ڲ�Ʒ�Ҳ���\n";
    std::cout << "\n2. ��Ʒ����˵��:\n";
    std::cout << "   - �Ƿ�У��: 1��ʾУ�ڿɻ�ã�0��ʾУ���Ʒ\n";
    std::cout << "   - �Ƿ���: 1��ʾ��ζ��Ʒ��0��ʾ������Ʒ\n";
    std::cout << "   - �������Ƿ�ʳ��: ������ʷ��¼�Զ�����\n";
    std::cout << "\n3. ���ѡ���Ʒ:\n";
    std::cout << "   - ���Ը��ݲ�Ʒ���ԣ��Ƿ�У�ڡ��Ƿ���������ɸѡ\n";
    std::cout << "   - �����ȷ�������ڲ����ظ��Ƽ�ͬһ����\n";
    std::cout << "\n4. ����ѡ���Ʒ:\n";
    std::cout << "   - ����ѡ����һ��ѡ��Ĳ�Ʒ\n";
    std::cout << "   - ��һ��ѡ��Ĳ�Ʒ�ᱻ���Ϊ������ʳ�ù�\n";
    std::cout << "\n5. �鿴���в�Ʒ:\n";
    std::cout << "   - ��ʾ���в�Ʒ��������\n";
    std::cout << "   - �����������Ƿ�ʳ�õ���Ϣ\n";
    std::cout << "\n6. ɾ��ָ����Ʒ:\n";
    std::cout << "   - ���ԴӲ�Ʒ�б���ɾ������Ҫ�Ĳ�Ʒ\n";
    std::cout << "\n7. ����²�Ʒ:\n";
    std::cout << "   - ͨ���˵�ѡ���������µĲ�Ʒ��dishes.txt�ļ�\n";
    std::cout << "\n8. ��ʷ��¼��ʽ:\n";
    std::cout << "   - ��ʷ��¼�������ڡ�ʱ�䡢���ڼ��Ͳ�Ʒ������Ϣ\n";
    std::cout << "   - ��ʽΪ: ʱ���|��Ʒ����|�Ƿ�У��|�Ƿ���|����ɶ�ʱ��\n";
    std::cout << "\n��Enter���������˵�...";
    std::cin.get();
}