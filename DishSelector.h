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
    // ��Ʒ�ṹ�壬�������ơ��Ƿ�У�ڡ��Ƿ������������Ƿ�ʳ��
    struct Dish {
        std::string name;
        bool isInSchool;
        bool isSpicy;
        bool eatenRecently = false;  // �������Ƿ�ʳ��
    };

    DishSelector();

    // ���ļ����ز�Ʒ�б�
    bool loadDishesFromFile(const std::string& filename);

    // ���ѡ��һ����Ʒ��ȷ�������ڲ��ظ�
    Dish selectRandomDish(bool inSchoolOnly = false,
                          bool spicyPreference = false);

    // ����ѡ���Ʒ��ȷ��������һ��ѡ�����ͬ
    Dish reselectDish(bool inSchoolOnly = false, bool spicyPreference = false);

    // ��ȡ���в�Ʒ�б�
    const std::vector<Dish>& getAllDishes() const;

    // ɾ��ָ����Ʒ
    bool deleteDish(const std::string& dishName);

    // ��ȡ��ǰ�Ѽ��صĲ�Ʒ����
    size_t getDishCount() const;

   private:
    // ��ʷ��¼�ṹ��
    struct DishHistory {
        std::string dishName;
        std::time_t timestamp;
        bool isInSchool;
        bool isSpicy;
    };

    bool loadHistory();                             // ������ʷ��¼
    void saveHistory(const std::string& dishName);  // ������ʷ��¼
    bool isDishSelectedRecently(
        const std::string& dishName) const;  // ����Ʒ�Ƿ��������ڱ�ѡ���

    std::vector<Dish> dishes;                       // �洢���в�Ʒ
    std::vector<DishHistory> history;               // �洢��ʷѡ���¼
    std::random_device rd;                          // ����������豸
    std::mt19937 gen;                               // �����������
    const std::string historyFile = "history.txt";  // ��ʷ��¼�ļ�·��
};

#endif  // DISH_SELECTOR_H