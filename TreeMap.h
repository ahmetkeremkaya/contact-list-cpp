#ifndef TREEMAP_H
#define TREEMAP_H

#include <iostream>
#include <vector>

#include "KeyValuePair.h"
#include "BinarySearchTree.h"

template<class K, class V>
class TreeMap {
public:
    TreeMap();

    void clear();
    const V &get(const K &key) const;
    const V &operator[](const K &key) const;

    void put(const K &key, const V &value);

    bool containsKey(const K &key);
    bool deletekey(const K &key);
    bool pop(const K &key);
    bool update(const K &key, const V &value);

    const KeyValuePair<K, V> &ceilingEntry(const K &key);
    const KeyValuePair<K, V> &floorEntry(const K &key);
    const KeyValuePair<K, V> &firstEntry();
    const KeyValuePair<K, V> &lastEntry();

    void pollFirstEntry();
    void pollLastEntry();
    
    void print() const;
    int size();

private:
    BinarySearchTree<KeyValuePair<K, V> > stree;
};

template<class K, class V>
TreeMap<K, V>::TreeMap() {}

template<class K, class V>
void TreeMap<K, V>::clear() {
    stree.removeAllNodes();
}

template<class K, class V>
const V &TreeMap<K, V>::get(const K &key) const {
    // 1. Adım: Elimizdeki "key" ile geçici bir KeyValuePair nesnesi oluşturuyoruz.
    // (Arama yaparken value'nun ne olduğunun önemi yok, sadece kılıfına uyduruyoruz)
    KeyValuePair<K, V> searchItem(key);

    // 2. Adım: Bu nesneyi ağacın get fonksiyonuna yolluyoruz.
    // stree.get() bize ağaçta bulduğu gerçek KeyValuePair nesnesini döndürür.
    // Biz de onun ".value" kısmını alıp kullanıcıya veriyoruz.
    return stree.get(searchItem).getValue();
}

template<class K, class V>
bool TreeMap<K, V>::pop(const K &key) {
     if(containsKey(key)){
         KeyValuePair<K,V> searchItem(key);
         stree.remove(searchItem);
    }
    else{
        return false; 
}}

template<class K, class V>
bool TreeMap<K, V>::update(const K &key, const V &value) {
    if (containsKey(key)){
        put(key,value);
        return true;
        
    }
    return false;
}

template<class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::ceilingEntry(const K &key) {
    KeyValuePair<K,V> searchItem(key);
    return stree.getCeiling(searchItem);
}

template<class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::floorEntry(const K &key) {
    KeyValuePair<K,V> searchItem(key);
    return stree.getFloor(searchItem);
}

template<class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::firstEntry() {
     
    return stree.getMin();
}

template<class K, class V>
const KeyValuePair<K, V> &TreeMap<K, V>::lastEntry() {
     return stree.getMax();
}

template<class K, class V>
void TreeMap<K, V>::pollFirstEntry() {
    deletekey(stree.getMin().getKey());
}

template<class K, class V>
void TreeMap<K, V>::pollLastEntry() {
    deletekey(stree.getMax().getKey());

}

template<class K, class V>
void TreeMap<K, V>::print() const {
    std::cout << "# TreeMap (BinarySearchTree):" << std::endl;
    stree.printPretty();
    std::cout << "# End TreeMap" << std::endl;
}

template<class K, class V>
int TreeMap<K, V>::size() {
    return stree.getSize();
}

template<class K, class V>
void TreeMap<K, V>::put(const K &key, const V &value) {
   // 1. Eklenecek yeni çifti oluştur (Hem key hem value ile)
    KeyValuePair<K, V> newItem(key, value);
    
    // 2. Eğer bu key zaten ağaçta varsa, önce eskisini silmemiz gerekir.
    if (containsKey(key)) {
        stree.remove(newItem); // Sadece key'e bakarak eskisini bulur ve siler.
    }

    // 3. Yeni değeri (güncellenmiş veya ilk kez gelen) ekle.
    stree.insert(newItem);
    
}

template<class K, class V>
const V &TreeMap<K, V>::operator[](const K &key) const {
    return get(key);
}

template<class K, class V>
bool TreeMap<K, V>::deletekey(const K &key) {
    if(containsKey(key)){
         KeyValuePair<K,V> searchItem(key);
         return stree.remove(searchItem);
    }
    else{
        return false; 
    }
    
}

template<class K, class V>
bool TreeMap<K, V>::containsKey(const K &key) {
    KeyValuePair<K,V> searchItem(key);
    
    return stree.contains(searchItem);
}






#endif
