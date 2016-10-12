#include <cstdio>
#include "io_helper.h"
#include <vector>
#include <algorithm>
using namespace std;
int main(int argc, char * argv[]) {
    ConstantLengthKmerHelper<uint64_t, uint16_t> iohelper(20,0);
    KmerFileReader< uint64_t,uint16_t > freader(argv[1], &iohelper);
    BinaryKmerWriter< uint64_t > fwriter(argv[2]);
    KVpair<uint64_t, uint16_t> pp;
    vector<uint64_t> VKmer;
    while (freader.getNext(&pp.k, &pp.v)) {
        fwriter.write(&pp.k);
        VKmer.push_back(pp.k);
    }
    fwriter.finish();
    printf("Sorting\n");
    sort(VKmer.begin(),VKmer.end());
    BinaryKmerReader< uint64_t > breader(argv[2]);
    while (breader.getNext(&pp.k)) {
    }

   return 0; 

}
