#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <inttypes.h>
#include "othelloindex.h"
#include <array>
#include <set>
using namespace std;
typedef uint64_t keyType;
typedef uint16_t valueType;
#define VALUELEN (16)
std::random_device rd;
std::uniform_int_distribution<keyType> disKey;
std::uniform_int_distribution<valueType> disValue;
std::mt19937_64 *g;
int main() {
    const int NN = 596;
    vector<keyType> kV;
    vector<valueType> vV;
    g = new std::mt19937_64 (rd());
    for (int i = 0; i < NN; i++) {
        kV.push_back(disKey(*g));
        valueType v = disValue(*g);
        v &= ((1<<VALUELEN)-1);
        vV.push_back(v);
    }
    set<valueType> mset;
    OthelloIndex <keyType> othidx(&kV[0],NN);
    for (int i = 0 ; i < NN; i++) {
        valueType t;
        t = othidx.query(kV[i]);
        mset.insert(t);
    }
    printf("%d\n", mset.size());
    if (mset.size()==NN && (*mset.begin())==0 && (*mset.rbegin())==NN-1 ) 
        printf("Test Succ!\n");
    else 
        printf("fail\n");
}
