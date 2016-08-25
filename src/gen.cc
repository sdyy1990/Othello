#include <iostream>
#include <cstdio>
const char * ac = "ATGC";
int main(int argc, char * argv[]) {
    int n,l,mx;
    sscanf(argv[1],"%d",&n);
    sscanf(argv[2],"%d",&l);
    sscanf(argv[3],"%d",&mx);
    for (int i = 0 ;i < n; i++) {
        for ( int j = 0 ; j < l; j++)
            printf("%c",  ac[ rand()%4]);
        printf(" %d\n",rand()%mx);
    }

}
