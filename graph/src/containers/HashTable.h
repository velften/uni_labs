#ifndef SEM3LAB3_HASHTABLE_H
#define SEM3LAB3_HASHTABLE_H

#include <cmath>
#include "IDictionary.h"
#include "../ptrs/UnqPtr.h"
#include "Hashing.h"

template<typename Tkey, typename Tvalue>
class HashTable: public IDictionary<Tkey, Tvalue>{

    struct Node{
        constexpr explicit Node() noexcept = default;
        int nextNodeIndx {-1};
        UnqPtr<Tvalue> value {nullptr};
        Tkey key {};

    };


    double loadFactor {0.0};

    const double MAX_LOAD_FACTOR = 0.75;

    size_t bucketsLoaded {0};

    size_t size {11};

    //using HashFunction = unsigned long (*)(const Tkey&);

    //HashFunction hasher {&defaultHasher};

    unsigned long hasher(const Tkey& key) const {
        std::hash<Tkey> hashFunc;
        return hashFunc(key) % size;
    }

    UnqPtr<Node[]> buckets;

    size_t searchNextNode(size_t currIndex, size_t currStep) const{
        currIndex += currStep*currStep;
        return currIndex % size;
    }

    void updateLoadFactor() {
        loadFactor = static_cast<double>(bucketsLoaded) / size;
    }

    bool isPrime(size_t num){
        if (num % 2 == 0)
            return false;
        for (size_t i = 3; i < static_cast<size_t>(std::sqrt(num)); i += 2){
            if (num % i == 0){
                return false;
            }
        }
        return true;
    }

    size_t calcNextPrimeNumber(size_t current){
        auto next = static_cast<size_t>(1.6 * current);
        for (next; next < 2 * current; ++next){
            if (isPrime(next)){
                return next;
            }
        }
    }

    void rebuild(){
        size_t newSize = calcNextPrimeNumber(size);
        HashTable<Tkey, Tvalue> newOne(newSize);
        for (size_t i = 0; i < size; i++){
            if (buckets[i].value.get() != nullptr){
                newOne.add(buckets[i].key, *(buckets[i].value));
            }
        }
        int op = 1;
        *this = std::move(newOne);
    }

    explicit HashTable(size_t size): size(size), buckets(new Node[size], size) {}

public:

    HashTable(): buckets(new Node[size], size) {}

    HashTable(const HashTable& other)
            : size(other.size), buckets(new Node[other.size], other.size),
              bucketsLoaded(other.bucketsLoaded), loadFactor(other.loadFactor) {
        for (size_t i = 0; i < size; ++i) {
            buckets[i].key = other.buckets[i].key;
            buckets[i].nextNodeIndx = other.buckets[i].nextNodeIndx;
            if (other.buckets[i].value.get()) {
                buckets[i].value.reset(new Tvalue(*(other.buckets[i].value)));
            }
        }
    }


    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            UnqPtr<Node[]> newBuckets(new Node[other.size], other.size);
            for (size_t i = 0; i < other.size; ++i) {
                newBuckets[i].key = other.buckets[i].key;
                newBuckets[i].nextNodeIndx = other.buckets[i].nextNodeIndx;
                if (other.buckets[i].value.get()) {
                    newBuckets[i].value.reset(new Tvalue(*(other.buckets[i].value)));
                }
            }
            size = other.size;
            bucketsLoaded = other.bucketsLoaded;
            loadFactor = other.loadFactor;
            buckets = std::move(newBuckets);
        }
        return *this;
    }


    HashTable(HashTable&& other) noexcept {
        buckets = std::move(other.buckets);
        loadFactor = other.loadFactor;
        size = other.size;
        bucketsLoaded = other.bucketsLoaded;
    }

    HashTable& operator=(HashTable&& other) noexcept {
        buckets = std::move(other.buckets);
        loadFactor = other.loadFactor;
        size = other.size;
        bucketsLoaded = other.bucketsLoaded;
        return *this;
    }


    int getCount() override{
        return bucketsLoaded;
    }

    int getCapacity() override{
        return size;
    }

    bool containsKey(const Tkey& key) const override{
        size_t currInd = hasher(key);
        size_t step = 1;
        while (buckets[currInd].value.get() != nullptr){
            if (buckets[currInd].key == key){
                return true;
            }
            currInd = searchNextNode(currInd,step);
            step++;
        }
        return false;
    }

    void add(const Tkey& key, const Tvalue& element) override{
        if (loadFactor >= MAX_LOAD_FACTOR){
            (*this).rebuild();
        }
        size_t currInd = hasher(key);
        size_t step = 1;
        while (buckets[currInd].value.get() != nullptr ){
            size_t nextIndex = searchNextNode(currInd, step);
            buckets[currInd].nextNodeIndx = nextIndex;
            currInd = nextIndex;
            step++;
        }
        buckets[currInd].key = key;
        buckets[currInd].value.reset(new Tvalue(element));
        bucketsLoaded++;
        updateLoadFactor();
    }

    Tvalue& operator[](const Tkey& key) override {
        size_t currInd = hasher(key);
        size_t step = 1;
        while (!(buckets[currInd].value.get() == nullptr && buckets[currInd].nextNodeIndx == -1)) {
            if (buckets[currInd].key == key) {
                return *(buckets[currInd].value);
            }
            currInd = searchNextNode(currInd, step);
            step++;
        }
        throw std::out_of_range("There is no such key in hashtable.");
    }

    Tvalue& operator[](const Tkey& key) const {
        size_t currInd = hasher(key);
        size_t step = 1;
        while (!(buckets[currInd].value.get() == nullptr && buckets[currInd].nextNodeIndx == -1)) {
            if (buckets[currInd].key == key) {
                return *(buckets[currInd].value);
            }
            currInd = searchNextNode(currInd, step);
            step++;
        }
        throw std::out_of_range("There is no such key in hashtable.");
    }

    Tvalue& get(const Tkey& key) override{
        return (*this)[key];
    }

    Tvalue get(const Tkey& key) const {
        return (*this)[key];
    }

    void remove(const Tkey& key) override{
        size_t currInd = hasher(key);
        size_t step = 1;
        while (buckets[currInd].value.get() != nullptr){
            if (buckets[currInd].key == key){
                buckets[currInd].value.reset();
                return;
            }
            currInd = searchNextNode(currInd,step);
            step++;
        }
        throw std::out_of_range("There is no such key in hashtable.");
    }


    class ConstIterator {
        const HashTable* table;
        size_t currentIndex{0};

    public:
        ConstIterator(const HashTable& tableRef, size_t index)
                : table(&tableRef), currentIndex(index) {
            moveToValidNode();
        }

        bool operator==(const ConstIterator& other) const {
            return table == other.table && currentIndex == other.currentIndex;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

        size_t getIndex() const {
            return currentIndex;
        }

        Tkey key() const {
            return table->buckets[currentIndex].key;
        }

        const Tvalue& operator*() const {
            return *(table->buckets[currentIndex].value.get());
        }

        const Tvalue* operator->() const {
            return table->buckets[currentIndex].value.get();
        }

        ConstIterator& operator++() {
            ++currentIndex;
            moveToValidNode();
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++(*this);
            return temp;
        }

    private:
        void moveToValidNode() {
            while (currentIndex < table->size &&
                   table->buckets[currentIndex].value.get() == nullptr) {
                ++currentIndex;
            }
        }
    };

    ConstIterator begin() const {
        return ConstIterator(*this, 0);
    }

    ConstIterator end() const {
        return ConstIterator(*this, size);
    }

    ~HashTable() = default;

};



#endif //SEM3LAB3_HASHTABLE_H