#pragma once
#include <vector>
#include <iostream>
#include <array>
#include "hash.h"
using namespace std;
template<uint8_t L, class keyType> 
class Othello
{
    typedef 
        typename conditional< L==8u , 
                 uint8_t, 
                 typename conditional< L==16u,
                          uint16_t,
                          typename conditional< L==32u,
                                   uint32_t,
                                   typename conditional< L==64u,
                                            uint64_t,
                                            array < uint8_t, L/8 >
                                            > :: type
                                   >::type
                          >::type
                 >::type valueType;
    
        
    vector<valueType> mem;
    valueType get(uint32_t loc); // 0..m_a-1 : arra, m_a..m_a+m_b-1 : arrb;
    void set(uint32_t loc,valueType &value);
    
public:
    Hasher32<keyType> *Ha;
    Hasher32<keyType> *Hb;
    bool build = false;
    pair<uint32_t, uint32_t> get_hash(const keyType &k);
    Othello(keyType *keys, valueType *_values, uint32_t keycount);
    Othello(vector<keyType> &keys, vector<valueType> &values) :
        Othello(&keys[0],&values[0],keys.size())
        {}
    inline valueType query(const keyType &k);


    void printValueTSize() {
        std::cout << sizeof(valueType) << std::endl;
    }
};


template<uint8_t L, class keyType> 
Othello(keyType *keys, valueType *_values, uint32_t keycount) {
    
}
