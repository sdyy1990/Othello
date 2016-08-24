#pragma once
#include "othello.h"
#include "io_helper.h"
#include <cstdio>
#include <cstdlib>

#include <cstring>
using namespace std;
const char * VERSION = GITVERSION;
#pragma message "VERSION: " GITVERSION
#define split(k,plow,phigh,spl) (((plow)=(k & ((1ULL << spl)-1))), ((phigh)=(k >> spl)))
template <uint8_t L, typename keyType>
class MulOth {
	vector<Othello<L, keyType> *> vOths;
#include "typedefine.h"
	unsigned char split;
public:
	bool buildsucc;
	MulOth(const char * fname, unsigned char _split) {
		printf("Building Multi Othello from file %s \n",fname);
		FILE *pFile;
		pFile = fopen (fname,"r");
		vOths.clear();
		split = _split;
		for (uint32_t pl = 0; pl < (1<<_split); pl++) {
			rewind(pFile);
			printf("Reading file for keys ending with %02x/%02x\n", pl,(1<<split)-1);
			vector<keyType> keys;
			vector<valueType> values;
			while (true) {
				char buf[1024];
				if (fgets(buf,1024,pFile)==NULL) break;
				keyType k;
				valueType v;
				if (!lineToKVpair(buf, &k, &v)) break;
				uint32_t plt;
				uint64_t ph;
				split(k,plt,ph,split);
				if (plt != pl) continue;
				keys.push_back(ph);
				values.push_back(v);
                //printf("%llx %llx %x %x\n",k,ph,plt,v);
			}
			printf("keycount %d ", keys.size());
            //for (int i = 0 ; i < keys.size(); i++) printf("%llx %x\n",keys[i],values[i]);
			Othello<L, keyType> *poth;
           poth = new Othello<L,keyType>(&keys[0], &values[0], keys.size());
			if (!poth->build) {
				printf("Build Halt!\n");
				return;
			}
			vOths.push_back(poth);
		}
		buildsucc = true;

	}

	MulOth(keyType * _keys, valueType * _values, uint64_t keycount, unsigned char _split) {
		buildsucc = false;
		vOths.clear();
		uint32_t plmax = (1<<_split) - 1;
		split = _split;
		for (uint32_t pl = 0; pl <= plmax; pl++) {
			vector<keyType> keys;
			vector<valueType> values;
			for (uint64_t i = 0; i < keycount; i++) {
				uint32_t plt;
				uint64_t ph;
				split(_keys[i],plt,ph,split);
				if (plt != pl) continue;
				keys.push_back(ph);
				values.push_back(_values[i]);
			}
			Othello<L, keyType> *poth;
            poth  = new Othello<L,keyType>(&keys[0], &values[0], keys.size());
			if (!poth->builtsucc) {
				printf("Build Halt!\n");
				return;
			}
			vOths.push_back(poth);
			//oth.printall();
		}
		buildsucc = true;
	}
	inline valueIntType query(const keyType &k) {
		uint32_t pl;
		uint64_t ph;
		split(k,pl,ph,split);
		return vOths[pl]->queryInt(ph);
	}
	void printall () {
		printf("Printall ...\n");
		for (auto V : vOths)
			V.printall();
	}
	void writeToFile(const char* fname) {
		FILE *pFile;
		pFile = fopen (fname, "wb");
		unsigned char buf0x20[0x20];
		char *p; p = (char *) &(buf0x20[0]);
		memset(buf0x20,0, sizeof(buf0x20));
		strcpy(p+0x4,VERSION);
		uint32_t split32 = split;
		memcpy(buf0x20, &split32, sizeof(uint32_t));
		fwrite(buf0x20,sizeof(buf0x20),1,pFile);
		for (int i = 0; i <(1<<split); i++) {
			vOths[i]->exportInfo(buf0x20);
			fwrite(buf0x20,sizeof(buf0x20),1,pFile);
		}
		for (int i = 0; i <(1<<split); i++) {
			fwrite(&(vOths[i]->mem[0]),sizeof(vOths[i]->mem[0]), vOths[i]->mem.size(), pFile);
		}
		fclose(pFile);
	}

	MulOth(const char* fname) {
		buildsucc = false;
		printf("Read from binary file %s\n", fname);
		FILE *pFile;
		pFile = fopen (fname, "rb");
		uint32_t compversion;
		unsigned char buf0x20[0x20];
		fread(buf0x20,sizeof(buf0x20),1,pFile);
		char *p; p = (char *) &(buf0x20[0]);
#ifndef NO_VERSION_CHECK
		if (strcmp(p+4,VERSION)) {
			printf("Wrong version number\n");
			fclose(pFile);
			return;
		}
#endif
		uint32_t split32;
		memcpy(&split32, buf0x20, sizeof(uint32_t));
		split = split32;
		for (int i = 0; i < (1<<split); i++) {
			fread(buf0x20,sizeof(buf0x20),1,pFile);
			Othello<L,keyType> *poth;
			poth = new Othello<L,keyType>(buf0x20);
			vOths.push_back(poth);
		}
		for (int i = 0; i < (1<< split); i++) {
			fread(&(vOths[i]->mem[0]),sizeof(vOths[i]->mem[0]), vOths[i]->mem.size(), pFile);
		}
		fclose(pFile);
		buildsucc = true;
	}
};


//MulOthello binary file descriptor
//0x00 : uint32_t splitbit
//0x04 : uint32_t signature MulOth version
//0x20 : OthInfo1
//0x40 : OthInfo2
//...
//offset1 : Oth[0].m
//offset2 = offset1 + Oth[0].hashupperbound = Oth[1].m
//...

//OthInfo: 32 Byte
//0x00 : uint64_t hash1
//0x08 : uint64_t hash2
//0x10 : uint32_t mask1
//0x14 : uint32_t mask2
//0x18 : uint64_t m.offset

