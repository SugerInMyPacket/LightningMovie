#ifndef MYMAP_MYMAP1_H
#define MYMAP_MYMAP1_H

#include <cstddef>

template<typename K, typename T>
class MyMapNode1{
private:
    K *key;
    T *value;
public:
    MyMapNode1(){
        key = nullptr;
        value = nullptr;
    }

    T* getValue()const {
        return value;
    }

    K* getKey() const {
        return key;
    }

    void setValue(const T& _value){
        if(value == nullptr){
            value = new T;
        }
        *value = _value;
    }

    void setKey(const K& _key){
        if(key == nullptr){
            key = new K;
        }
        *key = _key;
    }
};

template <typename K, typename T>
class MyMap1{
private:
    MyMapNode1<K,T> *array;
    size_t size;
    size_t maxSize;
public:
    explicit MyMap1(size_t _size){
        size = 0;
        maxSize = _size;
        array = new MyMapNode1<K,T>[maxSize];
    }

    virtual ~MyMap1(){
        delete [] array;
    }

    void add(const K& _key,
             const T& _value){
        if(size >= maxSize) return;
        array[size].setValue(_value);
        array[size++].setKey(_key);
    }

    T* search(const K& _key)const {
        for (size_t i = 0; i < size; ++i) {
            if(*(array[i].getKey()) == _key){
                return array[i].getValue();
            }
        }
        return nullptr;
    }
};

#endif //MYMAP_MYMAP1_H
