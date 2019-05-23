#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
#include <stdexcept>

constexpr static char NO_ELEMENT_VALUE = '.';
constexpr static char EXIST_ELEMENT_VALUE = '+';
constexpr static char DELETED_ELEMENT_VALUE = '-';

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    Hash hasher_;

    using listElement = std::pair<const KeyType, ValueType>;

    std::vector<std::pair<typename std::list<listElement>::iterator, char>> table_;
    std::list<listElement> elements_;
    size_t size_ = 0;
    size_t step_ = 1;

public:
    using iterator = typename std::list<listElement>::iterator;
    using const_iterator = typename std::list<listElement>::const_iterator;

    HashMap(const Hash& hash = Hash()) : hasher_(hash) {
        table_.resize(2, {iterator(), NO_ELEMENT_VALUE});
    }

    template<typename Iterator>
    HashMap(Iterator first, Iterator last, const Hash& hash = Hash()) : hasher_(hash) {
        table_.resize(std::distance(first, last) * 2, {iterator(), NO_ELEMENT_VALUE});
        while (first != last) {
            insert(*first++);
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> ilist, const Hash& hash = Hash()) {
        *this = HashMap(ilist.begin(), ilist.end(), hash);
    }

    HashMap(size_t size, const Hash& hash = Hash()) : hasher_(hash) {
        table_.resize(size, {iterator(), NO_ELEMENT_VALUE});
    }

    HashMap(const HashMap&) = default;

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    Hash hash_function() const {
        return hasher_;
    }

    void insert(const std::pair<KeyType, ValueType>& element) {
        auto bucketIndex = hasher_(element.first) % table_.size();
        while (table_[bucketIndex].second != NO_ELEMENT_VALUE) {
            if (table_[bucketIndex].second == EXIST_ELEMENT_VALUE &&
                table_[bucketIndex].first->first == element.first)
                return;
            bucketIndex += step_;
            if (bucketIndex >= table_.size()) {
                bucketIndex %= table_.size();
            }
        }

        size_++;

        bucketIndex = hasher_(element.first) % table_.size();
        while (table_[bucketIndex].second == EXIST_ELEMENT_VALUE) {
            bucketIndex += step_;
            if (bucketIndex >= table_.size()) {
                bucketIndex %= table_.size();
            }
        }
        elements_.push_back(element);

        auto last = elements_.end();
        --last;
        table_[bucketIndex] = make_pair(last, EXIST_ELEMENT_VALUE);

        if (size_ * 2 > table_.size()) {
            rehash(table_.size() * 2);
        }
    }

    void erase(KeyType key) {
        auto bucketIndex = hasher_(key) % table_.size();
        while (table_[bucketIndex].second != NO_ELEMENT_VALUE) {
            if (table_[bucketIndex].second == EXIST_ELEMENT_VALUE &&
                            table_[bucketIndex].first->first == key) {
                size_--;
                table_[bucketIndex].second = DELETED_ELEMENT_VALUE;
                elements_.erase(table_[bucketIndex].first);
                if (size_ * 4 <= table_.size()) {
                    rehash(table_.size() / 2);
                }
                return;
            }
            bucketIndex += step_;
            if (bucketIndex >= table_.size()) {
                bucketIndex %= table_.size();
            }
        }
    }

    void rehash(size_t newSize) {
        HashMap<KeyType, ValueType, Hash> new_table(newSize, hasher_);
        for (auto i : elements_) {
            new_table.insert(i);
        }

        std::swap(table_, new_table.table_);
        std::swap(elements_, new_table.elements_);
        std::swap(size_, new_table.size_);
        std::swap(step_, new_table.step_);
    }

    iterator begin() {
        return elements_.begin();
    }

    iterator end() {
        return elements_.end();
    }

    const_iterator begin() const {
        return elements_.cbegin();
    }

    const_iterator end() const {
        return elements_.cend();
    }

    iterator find_position(KeyType key) const {
        auto bucketIndex = hasher_(key) % table_.size();
        while (table_[bucketIndex].second != NO_ELEMENT_VALUE) {
            if (table_[bucketIndex].second == EXIST_ELEMENT_VALUE &&
                (*table_[bucketIndex].first).first == key) {
                return table_[bucketIndex].first;
            }
            bucketIndex += step_;
            if (bucketIndex >= table_.size()) {
                bucketIndex %= table_.size();
            }
        }
        return iterator{};
    }

    iterator find(KeyType key) {
        auto position = find_position(key);
        return position == iterator{} ? end() : position;
    }

    const_iterator find(KeyType key) const {
        auto position = find_position(key);
        return position == iterator{} ? end() :
                            static_cast<const_iterator>(position);
    }

    ValueType& operator[](KeyType key) {
        auto position = find(key);
        if (position != end()) {
            return position->second;
        }
        insert(std::make_pair(key, ValueType{}));
        auto last = elements_.end();
        --last;
        return (*last).second;
    }

    const ValueType& at(KeyType key) const {
        auto position = find(key);
        if (position != end()) {
            return position->second;
        }
        throw std::out_of_range("error");
    }

    void clear() {
        elements_.clear();
        table_.clear();
        size_ = 0;
        table_.resize(2, {iterator(), NO_ELEMENT_VALUE});
    }

    HashMap& operator=(HashMap new_table) {
        std::swap(table_, new_table.table_);
        std::swap(elements_, new_table.elements_);
        std::swap(size_, new_table.size_);
        std::swap(step_, new_table.step_);
        return *this;
    }
};
