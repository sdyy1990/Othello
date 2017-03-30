#include <iostream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "othello.h"
#include "muloth.h"
#include <chrono>
#include <inttypes.h>
#include <algorithm>
#include "io_helper.h"
#include "time.h"
#include <chrono>
using namespace std;
typedef unsigned long long keyT;
typedef uint16_t valueT;

keyT *keys;
valueT *values;

map<keyT,valueT> mapp;

bool inline check(Othello<keyT> & oth , keyT key, valueT value) {
             
    valueT q = (valueT) oth.queryInt(key);
    if (q != value) {
         printf("%llx: %lx!=%lx \n", key, q, value);
         return false;
    }
    return true;

}
int main(int argc, char * argv[]) {
    std::chrono::time_point<std::chrono::system_clock> clock1,clock2;
    std::chrono::duration<double> diffclock;
    int N;
    sscanf(argv[1],"%d",&N);
    N*=1024;
    int Nmax = (N*3);
    keys = (keyT*) valloc(sizeof(keyT) * Nmax);
    values = (valueT*) valloc(sizeof(valueT)*Nmax);
    for (int i = 0 ; i < Nmax; i++) {
        keys[i] = ((((unsigned long long) rand())<<32) + rand() | 0x1LL);
        values[i] = (rand()&0xFFFF);
    }
    clock2 = std::chrono::system_clock::now();    clock1 = clock2;
    Othello<keyT> oth(16,&keys[0],N, false, &values[0], sizeof(values[0]),0);
    clock2 = std::chrono::system_clock::now(); diffclock = clock2 - clock1;    clock1 = clock2;
    printf("Othello keycount = %d\n", oth.mykeycount);
    printf("Time used %.3lf\n", diffclock.count());

    int updatecount;
    sscanf(argv[2],"%d",&updatecount);
    updatecount *= 1024;
    for (int t = 0 ; t < updatecount; t++) {
        if ((rand() & 1) == 0)  {
            int i = oth.mykeycount;
            keys[i] = ((((unsigned long long) rand())<<32) + rand() | 0x1LL);
            values[i] = (rand()&0xFFFF);
            oth.addkeys(1, &(values[0]), sizeof(values[0]));
        }
        else {
            int i = rand() % oth.mykeycount;
            values[i] = values[oth.mykeycount-1];
            oth.removeOneKey(i);
        }
    }
    printf("After %d K updates, Othello keycount = %d\n", updatecount>>10, oth.mykeycount);
    printf("Time used %.3lf\n", diffclock.count());
    /*
    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    printf("Add 100 keys\n");
    oth.addkeys(100, &(values[0]), sizeof(values[0]));
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 


    printf("Remove one key currently at location 0 \n");
    values[0] = values[oth.mykeycount-1];
    oth.removeOneKey(0);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 


    printf("Remove one key currently at location 5 \n");
    values[5] = values[oth.mykeycount-1];
    oth.removeOneKey(5);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 


    for (int i = oth.mykeycount ; i < oth.mykeycount+200; i++) {
        keys[i] = (((unsigned long long) rand())<<32) + rand();
        values[i] = rand();
    }

    printf("Add 200 keys\n");
    oth.addkeys(200, &(values[0]), sizeof(values[0]));
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
 
    printf("Modify 10 random values\n");
    for (int t = 0; t < 10; t++) {
        int i = rand() % oth.mykeycount;
        values[i] = (rand()&0xFFFF);
        oth.updatevalue(i,&(values[0]),sizeof(values[0]));
        if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    }*/
    free(keys);
    free(values);
    return 0;
}
