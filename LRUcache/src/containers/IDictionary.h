#ifndef SEM3LAB3_IDICTIONARY_H
#define SEM3LAB3_IDICTIONARY_H

template <typename Tkey, typename Tvalue>
class IDictionary {
public:
    virtual int getCount() = 0;
    virtual int getCapacity() = 0;
    virtual Tvalue& get(const Tkey& key) = 0;
    virtual bool containsKey(const Tkey& key) const = 0;
    virtual void add(const Tkey& key, const Tvalue& element) = 0;
    virtual Tvalue& operator[] (const Tkey& key) = 0;
    virtual void remove(const Tkey& key) = 0;

    virtual ~IDictionary() {};
};
#endif //SEM3LAB3_IDICTIONARY_H
