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
    const int NN = 96;
    vector<keyType> kV;
    vector<valueType> vV;
    g = new std::mt19937_64 (rd());
    for (int i = 0 ; i < NN; i++) {
        kV.push_back(disKey(*g));
        valueType v = disValue(*g);
        v &= ((1<<VALUELEN)-1);
        vV.push_back(v);
    }
    Othello<VALUELEN, keyType> oth(kV, vV,false);

    array< uint32_t, VALUELEN*2> cnt;
    array<uint32_t,VALUELEN> ans;
    array< double, VALUELEN> rat;
    
    cnt  = oth.getCnt();
    rat  = oth.getRatio();
    for (int i = 0; i < VALUELEN; i++) {  printf("%d ", cnt[i]);   }    printf("\n");
    for (int i = 0; i < VALUELEN; i++) {        printf("%d ", cnt[VALUELEN+i]);    }    printf("\n");
    for (int i = 0; i < VALUELEN; i++) {        printf("%.3lf ", rat[i]);   }    printf("\n");
    const int CCNT = 10000000;
    for (int j = 0 ; j < VALUELEN; j++) ans[j] =0;
    for (int i = 0 ; i < CCNT; i++) {
        keyType T = disKey(*g);
        valueType V = oth.query(T);
        for (int j = 0; j < VALUELEN; j++)
            ans[j] += ((V >> j) & 1);
    }
    printf("Query Rates:");
    for (int i = 0 ; i < VALUELEN; i++)
        printf("%.3lf ",ans[i]*1.0/CCNT);
    printf("\n");


    for (int i = 0 ; i < kV.size(); i++) {
        valueType v = oth.query(kV[i]);
        if (v!=vV[i]) {            printf("Err!!!!");            return 0;
        }
    }
    printf("Verify Succ\n");


    printf("AlienPreference");
    oth.setAlienPreference(0.0);
    cnt  = oth.getCnt();
    rat  = oth.getRatio();
    for (int j = 0 ; j < VALUELEN; j++) ans[j] =0;
    for (int i = 0; i < VALUELEN; i++) {  printf("%d ", cnt[i]);   }    printf("\n");
    for (int i = 0; i < VALUELEN; i++) {        printf("%d ", cnt[VALUELEN+i]);    }    printf("\n");
    for (int i = 0; i < VALUELEN; i++) {        printf("%.3lf ", rat[i]);   }    printf("\n");
    for (int i = 0 ; i < CCNT; i++) {
        keyType T = disKey(*g);
        valueType V = oth.query(T);
        for (int j = 0; j < VALUELEN; j++)
            ans[j] += ((V >> j) & 1);
    }
    printf("Query Rates:");
    for (int i = 0 ; i < VALUELEN; i++)
        printf("%.3lf ",ans[i]*1.0/CCNT);
    printf("\n");

    for (int i = 0 ; i < kV.size(); i++) {
        valueType v = oth.query(kV[i]);
        if (v!=vV[i]) {            printf("Err!!!!");            return 0;
        }
    }
    printf("Verify Succ\n");
    return 0;
}
