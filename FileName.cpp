#include <iostream>
#include <vector>
#include <list>
#include <string>

// структура Order
struct Order {
    int number; // ключ
    std::string customer; // значение
};

// хэш-таблица
class HashTable {
private:
    std::vector<std::list<Order>> table; // вектор списков для хранения данных
    size_t capacity; // размер таблицы
    size_t elementCount; // количество элементов в таблице

    // хэш-функция: XOR ключа и длины строки customer
    size_t hashFunction(int key, const std::string& customer) const {
        size_t hash = key ^ customer.length(); // XOR ключа и длины строки
        return hash % capacity;
    }

    // получение индекса по ключу
    size_t getIndex(int key, const std::string& customer) const {
        return hashFunction(key, customer);
    }

    // рехэширование (увеличение размера таблицы)
    void rehash() {
        size_t newCapacity = capacity * 2; // увеличиваем размер таблицы в 2 раза
        std::vector<std::list<Order>> newTable(newCapacity); // новая таблица

        // рехеширование всех элементов
        for (const auto& chain : table) {
            for (const auto& order : chain) {
                size_t newIndex = hashFunction(order.number, order.customer) % newCapacity;
                newTable[newIndex].push_back(order);
            }
        }

        // заменяем старую таблицу новой
        table = std::move(newTable);
        capacity = newCapacity;
    }

public:
    // конструктор
    HashTable(size_t initialCapacity = 10) : capacity(initialCapacity), elementCount(0) {
        table.resize(capacity);
    }

    // деструктор
    ~HashTable() {
        clear(); // очищаем таблицу при уничтожении объекта
    }

    // вставка элемента
    void insert(const Order& order) {
        // проверяем, нужно ли рехэширование
        if (elementCount >= capacity * 0.75) { // Если заполнено на 75%
            rehash();
        }

        size_t index = getIndex(order.number, order.customer);
        table[index].push_back(order);
        elementCount++;
    }

    // поиск элемента
    bool find(int key, const std::string& customer, Order& result) const {
        size_t index = getIndex(key, customer);
        for (const auto& order : table[index]) {
            if (order.number == key && order.customer == customer) {
                result = order;
                return true;
            }
        }
        return false;
    }

    // удаление элемента
    bool remove(int key, const std::string& customer) {
        size_t index = getIndex(key, customer);
        auto& chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->number == key && it->customer == customer) {
                chain.erase(it);
                elementCount--;
                return true;
            }
        }
        return false;
    }

    // вывод хэш-таблицы
    void print() const {
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "bucket " << i << ": ";
            for (const auto& order : table[i]) {
                std::cout << "[" << order.number << ", " << order.customer << "] ";
            }
            std::cout << std::endl << std::endl;
        }
    }

    // очистка таблицы
    void clear() {
        for (auto& chain : table) {
            chain.clear();
        }
        elementCount = 0;
    }

    // Количество элементов в таблице
    size_t size() const {
        return elementCount;
    }

    // Проверка на пустоту
    bool isEmpty() const {
        return elementCount == 0;
    }
};
