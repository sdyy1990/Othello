#include <iostream>
#include <type_traits>
#include <cstdint>
#include "hash.h"
using namespace std;
int main() {
    int A = 1234;
    Hasher32<int> hA(0x5,0x1);
    cout << hA(A);
    


}
