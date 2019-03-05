#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
#include <stdexcept>

using namespace std;


template<class KeyType, class ValueType, class Hash = hash<KeyType> > class HashMap {
private:
    Hash tableHasher;

    using listValue = pair<const KeyType, ValueType>;

    vector<pair<typename list<listValue>::iterator, char>> table;
    list<listValue> elements;
    size_t tableSize = 0, tableStep = 1;

public:
    using iterator = typename list<listValue>::iterator;
    using const_iterator = typename list<listValue>::const_iterator;

    HashMap(const Hash& hsh = Hash()) : tableHasher(hsh){
        table.resize(2, {iterator(), '.'});
    }

    HashMap(iterator first, iterator last, const Hash& hsh = Hash()) : tableHasher(hsh) {
        auto old_first = first;
        size_t sz = 0;
        while (first != last) {
            ++sz;
            ++first;
        }
        table.resize(sz * 2, {iterator(), '.'});
        first = old_first;
        while (first != last) {
            insert((*first));
            ++first;
        }
    }

    HashMap(const initializer_list<pair<KeyType, ValueType>>& arr, const Hash& hsh = Hash()) :
            tableHasher(hsh) {
        auto old_first = arr.begin(), last = arr.end();
        auto first = old_first;
        size_t sz = 0;
        while (first != last) {
            ++sz;
            ++first;
        }
        table.resize(sz * 2, {iterator(), '.'});
        first = old_first;
        while (first != last) {
            insert((*first));
            ++first;
        }
    }

    HashMap(size_t sz, const Hash& hsh = Hash()) : tableHasher(hsh) {
        table.resize(sz, {iterator(), '.'});
    }

    size_t size() const {
        return tableSize;
    }

    bool empty() const {
        return tableSize == 0;
    }

    Hash hash_function() const {
        return tableHasher;
    }

    void insert(pair<KeyType, ValueType> element) {
        auto ind = tableHasher(element.first) % table.size();
        while (table[ind].second != '.') {
            if (table[ind].second == '+' &&
                    table[ind].first->first == element.first)
                return;
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }

        tableSize++;

        ind = tableHasher(element.first) % table.size();
        while (table[ind].second == '+') {
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }
        elements.push_back(element);

        auto last = elements.end();
        --last;
        table[ind] = make_pair(last, '+');


        if (tableSize * 2 > table.size()) {
            move_to_new_table(table.size() * 2);
        }
    }

    void erase(KeyType key) {
        auto ind = tableHasher(key) % table.size();
        while (table[ind].second != '.') {
            if (table[ind].second == '+' &&
                    table[ind].first->first == key) {
                tableSize--;
                table[ind].second = '-';
                elements.erase(table[ind].first);
                if (tableSize * 4 <= table.size())
                    move_to_new_table(table.size() / 2);
                return;
            }
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }
    }

    void move_to_new_table(size_t newSize) {
        HashMap<KeyType, ValueType, Hash> new_table(newSize, tableHasher);
        for (auto i : elements) {
            new_table.insert(i);
        }

        swap(table, new_table.table);
        swap(elements, new_table.elements);
        swap(tableSize, new_table.tableSize);
        swap(tableStep, new_table.tableStep);
    }

    iterator begin() {
        return elements.begin();
    }

    iterator end() {
        return elements.end();
    }

    const_iterator begin() const {
        return elements.cbegin();
    }

    const_iterator end() const {
        return elements.cend();
    }

    iterator find(KeyType key) {
        auto ind = tableHasher(key) % table.size();
        while (table[ind].second != '.') {
            if (table[ind].second == '+' &&
                (*table[ind].first).first == key) {
                return table[ind].first;
            }
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }
        return end();
    }

    const_iterator find(KeyType key) const {
        auto ind = tableHasher(key) % table.size();
        while (table[ind].second != '.') {
            if (table[ind].second == '+' &&
                (*table[ind].first).first == key) {
                return table[ind].first;
            }
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }
        return end();
    }

    ValueType& operator [](KeyType key) {
        auto ind = tableHasher(key) % table.size();
        while (table[ind].second != '.') {
            if (table[ind].second == '+' &&
                table[ind].first->first == key)
                return table[ind].first->second;
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }
        insert(make_pair(key, ValueType{}));
        auto last = elements.end();
        --last;
        return (*last).second;
    }

    const ValueType& at(KeyType key) const {
        auto ind = tableHasher(key) % table.size();
        while (table[ind].second != '.') {
            if (table[ind].second == '+' &&
                table[ind].first->first == key)
                return table[ind].first->second;
            ind += tableStep;
            if (ind >= table.size())
                ind %= table.size();
        }
        throw out_of_range("kekos");
    }

    void clear() {
        elements.clear();
        table.clear();
        tableSize = 0;
        table.resize(2, {iterator(), '.'});
    }

    HashMap<KeyType, ValueType, Hash>& operator =(HashMap<KeyType, ValueType, Hash> new_table) {
        swap(table, new_table.table);
        swap(elements, new_table.elements);
        swap(tableSize, new_table.tableSize);
        swap(tableStep, new_table.tableStep);
        return *this;
    }

    ~HashMap() {}
};
