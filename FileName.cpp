#include <iostream>
#include <vector>
#include <list>
#include <string>

// ��������� Order
struct Order {
    int number; // ����
    std::string customer; // ��������
};

// ���-�������
class HashTable {
private:
    std::vector<std::list<Order>> table; // ������ ������� ��� �������� ������
    size_t capacity; // ������ �������
    size_t elementCount; // ���������� ��������� � �������

    // ���-�������: XOR ����� � ����� ������ customer
    size_t hashFunction(int key, const std::string& customer) const {
        size_t hash = key ^ customer.length(); // XOR ����� � ����� ������
        return hash % capacity;
    }

    // ��������� ������� �� �����
    size_t getIndex(int key, const std::string& customer) const {
        return hashFunction(key, customer);
    }

    // ������������� (���������� ������� �������)
    void rehash() {
        size_t newCapacity = capacity * 2; // ����������� ������ ������� � 2 ����
        std::vector<std::list<Order>> newTable(newCapacity); // ����� �������

        // ������������� ���� ���������
        for (const auto& chain : table) {
            for (const auto& order : chain) {
                size_t newIndex = hashFunction(order.number, order.customer) % newCapacity;
                newTable[newIndex].push_back(order);
            }
        }

        // �������� ������ ������� �����
        table = std::move(newTable);
        capacity = newCapacity;
    }

public:
    // �����������
    HashTable(size_t initialCapacity = 10) : capacity(initialCapacity), elementCount(0) {
        table.resize(capacity);
    }

    // ����������
    ~HashTable() {
        clear(); // ������� ������� ��� ����������� �������
    }

    // ������� ��������
    void insert(const Order& order) {
        // ���������, ����� �� �������������
        if (elementCount >= capacity * 0.75) { // ���� ��������� �� 75%
            rehash();
        }

        size_t index = getIndex(order.number, order.customer);
        table[index].push_back(order);
        elementCount++;
    }

    // ����� ��������
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

    // �������� ��������
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

    // ����� ���-�������
    void print() const {
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "bucket " << i << ": ";
            for (const auto& order : table[i]) {
                std::cout << "[" << order.number << ", " << order.customer << "] ";
            }
            std::cout << std::endl << std::endl;
        }
    }

    // ������� �������
    void clear() {
        for (auto& chain : table) {
            chain.clear();
        }
        elementCount = 0;
    }

    // ���������� ��������� � �������
    size_t size() const {
        return elementCount;
    }

    // �������� �� �������
    bool isEmpty() const {
        return elementCount == 0;
    }
};
