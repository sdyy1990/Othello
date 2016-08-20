#pragma once
#include <vector>
#include <iostream>
template<uint8_t L, class keyType> 
class Othello
{
    typedef 
        typename std::conditional< L==8u , 
                 uint8_t, 
                 typename std::conditional< L==16u,
                          uint16_t,
                          typename std::conditional< L==32u,
                                   uint32_t,
                                   typename std::conditional< L==64u,
                                            uint64_t,
                                            uint8_t [L/8]
                                            > :: type
                                   >::type
                          >::type
                 >::type valueType;
        
public:
    void printValueTSize() {
        std::cout << sizeof(valueType) << std::endl;
    }
};
