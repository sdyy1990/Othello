#pragma once
template <class Key>
class Hasher32 {
    std::hash<Key> fallback;
    uint32_t mask;
    uint32_t s;
    uint32_t hashshr;
public:
    Hasher32(uint32_t hashlength, uint32_t _seed) {
       mask = (1U << hashlength) -1;
       s = _seed;
       hashshr = s & 7;
    }
    template <class T = Key>
    typename std::enable_if<std::is_integral<T>::value, uint32_t>::type
    operator()(const Key& k0) const {
#if defined(__SSE4_2__)
#pragma message("Hasher CRC32c");
        uint32_t crc1 = ~0;
        uint64_t *k;
        k = (uint64_t* ) &k0;
        uint32_t s1 = s;
        if (sizeof(Key)>=8)
            asm(".byte 0xf2, 0x48, 0xf, 0x38, 0xf1, 0xf1;"  :"=S"(crc1)   :"0"(crc1), "c" ((*k)+s1)  );

        if (sizeof(Key)>=16) {
            s1 = ((((uint64_t) s1) * s1 >> 16) ^ (s1 << 2));
            k++;
            asm(".byte 0xf2, 0x48, 0xf, 0x38, 0xf1, 0xf1;"  :"=S"(crc1)   :"0"(crc1), "c" ((*k)+s1)  );
        }
        if (sizeof(Key)>=24) {
            s1 = ((((uint64_t) s1) * s1 >> 16) ^ (s1 << 2));
            k++;
            asm(".byte 0xf2, 0x48, 0xf, 0x38, 0xf1, 0xf1;"  :"=S"(crc1)   :"0"(crc1), "c" ((*k)+s1)  );
        }
        if (sizeof(Key)>=32) {
            s1 = ((((uint64_t) s1) * s1 >> 16) ^ (s1 << 2));
            k++;
            asm(".byte 0xf2, 0x48, 0xf, 0x38, 0xf1, 0xf1;"  :"=S"(crc1)   :"0"(crc1), "c" ((*k)+s1)  );
        }
        if (sizeof(Key) & 4) {
            uint64_t k32;
            k32 =  ((*k)) & 0xFFFFFFFFULL;
            asm( ".byte 0xf2, 0xf, 0x38, 0xf1, 0xf1;"  :"=S"(crc1)    :"0" (crc1),"c" ((k32)+s1)  );

        }

        asm( ".byte 0xf2, 0xf, 0x38, 0xf1, 0xf1;"  :"=S"(crc1)    :"0" (crc1),"c" (s1)  );
//    crc1 ^= (crc1 >> (HASHLENGTH ^ (7&s1)));
        crc1 ^= (crc1 >> (hashshr));
        if (sizeof(Key)==4)
            return mask & (crc1 ^ ((uint32_t) *k));
        else
            return mask & (crc1 ^ (*k >> 32) ^ ((uint32_t) *k));
#else
#pragma message("Build without SSE4.2 support ")
        return mask & fallback(k0);
#endif
    }

    template <class T = Key>
    typename std::enable_if<!std::is_integral<T>::value, uint32_t>::type
    operator()(const Key& k0) const {
        return mask & fallback(k0^s ^ (s <<(8+hashshr)));
    }

};

