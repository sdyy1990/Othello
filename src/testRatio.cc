#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <inttypes.h>
#include "othello.h"
#include <array>
using namespace std;
typedef uint64_t keyType;
typedef uint8_t valueType;
#define VALUELEN (4)
std::random_device rd;
std::uniform_int_distribution<keyType> disKey;
std::uniform_int_distribution<valueType> disValue;
std::mt19937_64 *g; 
int main() {
    const int NN = 2048;
    vector<keyType> kV;
    vector<valueType> vV;
    g = new std::mt19937_64 (rd());
    for (int i = 0 ; i < NN; i++) {
        kV.push_back(disKey(*g));
        vV.push_back(disValue(*g));
    }
    Othello<VALUELEN, keyType> oth(kV, vV);
    array< uint32_t, VALUELEN*2> cnt = oth.getCnt();
    array< double, VALUELEN> rat = oth.getRatio();
    for (int i = 0; i < VALUELEN; i++) {
        printf("%d ", cnt[i]);
    }
    printf("\n");
    for (int i = 0; i < VALUELEN; i++) {
        printf("%d ", cnt[VALUELEN+i]);
    }
    printf("\n");
    for (int i = 0; i < VALUELEN; i++) {
        printf("%.3lf ", rat[i]);
    }
    printf("\n");
    const int CCNT = 1000000;
    array<uint32_t,VALUELEN> ans;
    for (int i = 0 ; i < CCNT; i++) {
        keyType T = disKey(*g);
        valueType V = oth.query(T);
        for (int i = 0; i < VALUELEN; i++)
            ans[i] += ((V >> i) & 1);
    }
    for (int i = 0 ; i < VALUELEN; i++)
        printf("%.3lf ",ans[i]*1.0/CCNT);
    printf("\n");
    return 0;
}
