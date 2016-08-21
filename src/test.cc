#include <iostream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "othello.h"
#include "muloth.h"
#include <chrono>
#include <inttypes.h>
#include "io_helper.h"
using namespace std;
typedef unsigned long long keyT;
typedef uint8_t valueT;

vector<keyT> keys;
vector<valueT> values;


void add(char * fname) {
    printf("Reading file %s \n" , fname);
    FILE *pFile;
    pFile = fopen (fname,"r");
    uint64_t cnt = 0 ;
    while (true) {
        char buf[1024];
        if (fgets(buf,1024,pFile)==NULL) break;
        keyT k;
        valueT v;
        if (!lineToKVpair(buf, &k , &v)) break;
        keys.push_back(k);
        values.push_back(v);
        cnt++;
    }
    fclose(pFile);
    printf("key cnt %lld; tot key %lld \n", cnt, keys.size());
}

void test(char * fname, MulOth<keyT,valueT> & moth) {
}

int main(int argc, char * argv[]) {
    if (argc < 4) {
        printf(" splitbits keyInputFile OutputFile .... \n");
        printf(" splitbits: a number <=16, divide the keys into 2^(splitbits) sets, according to the Least (splitbit) significant bits. \n");
        printf(" splitbit == -1: -1, keyInputfile, outputfile: skip build, just query\n");
        return 0;
    }
    int splitbit;
    sscanf(argv[1],"%d",&splitbit);
    MulOth<keyT,valueT> * moth;
    if (splitbit >=0) {
    printf("Split %d groups\n",1U<< splitbit);
#ifdef BATCH_READ_MUL_OTHELLO
    printf("Reading files ...\n");
    for (int i = 2; i< 3; i++) {
        add (argv[i]);
    }
    //for (int i = 0 ; i < keys.size(); i++) {
    //    printf("%" PRIx64 "-> %d\n", keys[i], values[i]);
    //}
    mont = new MulOth<keyT,valueT>(&keys[0],&values[0],keys.size(),splitbit);
#else
    moth = new MulOth<keyT,valueT>(argv[2],  splitbit);
#endif    
    printf("Build Succ, write to file %s\n", argv[3]);
    //moth.printall();
    moth->writeToFile(argv[3]);
    delete moth;
    }
    moth = new MulOth<keyT,valueT>( argv[3]);

    for (int i = 2; i< argc; i+=2) {
        printf("Testing using keys from file %s\n" , argv[i]);
        FILE *pFile;
        pFile = fopen (argv[i],"r");
        uint64_t cnt = 0 ;
        while (true) {
            char buf[1024];
            if (fgets(buf,1024,pFile)==NULL) break;
            keyT k;
            valueT v;
            if (!lineToKVpair<keyT,valueT>(buf, &k , &v)) break;
            valueT qv = moth->query(k);
            if (qv !=v ) {
                printf("Err %" PRIx64 "->%d: %d\n", k,v,qv);
                fclose(pFile);
                return 0;
            }
            //    printf("%" PRIx64"--> %d\n", k,v);

        }
        fclose(pFile);
        printf("Test Succ\n");
    }
    return 0;
}
