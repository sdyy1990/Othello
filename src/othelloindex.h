#pragma once
#include "othello.h"
/*!
  \file othellotable.h
  Describes the data structure *OthelloIndex*
*/

/*!
    \brief OthelloIndex is a data structure that stores a minimum perfect hash function. 
    \n For n keys, the query value of keys range in [0..n-1]
    Query always returns uint32_t.
    \note n should not exceed 2^29 for memory consideration. For even larger n, use the grouped version *MulOthIndex*.
    The index of a key k is either Sum {cntfill[0..Ha(k)-1]} or Sum {cntfill[0..Hb(k)-1]}. This is decided by query result on *oth*.

 */
template <class keyType>
class OthelloIndex {
    Othello<1, keyType> *oth;
    vector<uint32_t> offset;
    uint32_t sum(uint32_t h) {
        uint32_t p = (h & 0x1F);
        return offset[h>>5] + __builtin_popcount(oth->fillcount[h>>5] & ((1<<p)-1));
    }
public:    
    bool build = false; //!< true if OthelloIndex is successfully built.
    /*! 
     \brief Construct *OthelloIndex*
     \param [in] keyType * keys, pointer to array of keys.
     \param [in] uint32_t keycount, number of keys.
     */
    OthelloIndex(keyType *_keys, uint32_t keycount) {
        oth = new Othello<1,keyType>(_keys, keycount);
        build = oth->build;
        offset.resize(oth->fillcount.size());
        offset[0] = 0;
        for (int i = 1 ; i < offset.size(); i++) {
            offset[i] = offset[i-1] + __builtin_popcount(oth->fillcount[i-1]);
        }
          

    } 
    ~OthelloIndex() {
        delete oth;
        offset.clear();     
    }
    /*!
     \brief return the index of key k, in range [0..n-1]
     \param [in] keyType k
     \retval uint32_t index
     */
    uint32_t query(const keyType &k) {
        uint32_t ha,hb;
        uint8_t v = oth->query(k,ha,hb);
        uint32_t h = v?hb:ha;
        return sum(h);
    }
};



