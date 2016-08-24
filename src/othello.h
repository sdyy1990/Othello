#pragma once
#include <vector>
#include <iostream>
#include <array>
#include <queue>
#include <cstring>
#include "hash.h"
#include "io_helper.h"
using namespace std;

class DisjointSet {
	int siz;
	vector<int32_t> *fa;
	uint32_t getfa(int i) {
		if ((*fa)[i] < 0) (*fa)[i] = i;
		else if ((*fa)[i]!=i)
			(*fa)[i] = getfa((*fa)[i]);
		return (*fa)[i];
	}
public:
	void finish() {
		fa->clear();
		delete fa;
	}
	void setLength(int n) {
		fa = new vector<int32_t> (n,-1);
	}
	void clear() {
		for (auto a : *fa)
			a = 0;
	}
	void merge(int a, int b) {
		if (a==0) swap(a,b);  //b!=0
		(*fa)[getfa(a)] = getfa(b);
	}
	bool sameset(int a, int b) {
		return getfa(a)==getfa(b);
	}
	bool isroot(int a) {
		return ((*fa)[a]==a);
	}
};


template<uint8_t L, class keyType>
#define MAX_REHASH 100
class Othello
{
//TESTCODE
#include "typedefine.h"
public:
	vector<valueType> mem;
	uint32_t ma;
	uint32_t mb;
	Hasher32<keyType> *Ha;
	Hasher32<keyType> *Hb;
	bool build = false;
	uint32_t trycount = 0;
	DisjointSet disj;

	inline valueType get(uint32_t loc) { // 0..m_a-1 : arra, m_a..m_a+m_b-1 : arrb;
		if ( sizeof(mem[0])*8 == L) {
			return mem[loc];
		} else {
			//partial;  e.g, 33221100
			const uint32_t percell = sizeof(mem[0])*8/L;
			return (mem[loc/percell] >> ((loc % percell)*L))&((1<<L)-1);

		}
	}
	inline void set(uint32_t loc,valueType &value) {
		if (sizeof(mem[0])*8 == L) {
			mem[loc] = value;
		} else {
			//partial;
			const uint32_t percell = sizeof(mem[0])*8/L;
			uint32_t mv = loc % percell;
			// 33221100--> 3322xx00, changedbits = 11^xx
			valueType mask1 = ~(((1<<L)-1) << (mv*L));
			mem[loc] &= mask1;
			mem[loc] ^= (value << (mv*L));
		}
	}

	template<class VT = valueType>
	inline typename std::enable_if< std::is_same<VT,array<uint8_t,L/8> >::value, VT>::type
	getrand(array<uint8_t,L/8> &v) {
		array<uint8_t, L/8> ret;
		for (int i = 0; i < L/8; i++) ret[i] = rand();
	}

	template<class VT = valueType>
	inline typename std::enable_if<std::is_same<VT, uint64_t >::value || std::is_same<VT, uint32_t>::value, VT>::type
	getrand(valueType &v) {
		valueType va = rand();
		va <<=28;
		va ^= rand();
		va <<=28;
		v = (va ^ rand());
	}

	template<class VT = valueType>
	inline typename std::enable_if<
	    std::is_same<VT, uint16_t>::value ||
	    std::is_same<VT, uint8_t>::value
	    , VT>::type
	getrand(valueType &v) {
		v = rand();
	}


	void newHash() {
		Ha->setSeed(rand());
		Hb->setSeed(rand());
		trycount++;
	}

	vector<int32_t> *first, *nxt1, *nxt2;
	bool testHash(keyType *keys, uint32_t keycount);

	void fillvalue(keyType *keys, valueType *values, uint32_t keycount) {
		vector<bool> filled;
		filled.resize(ma+mb);
		for (int i = 0; i< ma+mb; i++)
			if (disj.isroot(i)) {
				queue<uint32_t> Q;
				Q.push(i);
				valueType vv;
				getrand(vv);
				set(i,vv);
				filled[i] = true;
				while (!Q.empty()) {
					uint32_t nodeid = (Q.front());
					Q.pop();
					vector<int32_t> *nxt;
					if (nodeid < ma) nxt = nxt1;
					else nxt = nxt2;
					int32_t kid = first->at(nodeid);
					while (kid >=0) {
						uint32_t ha,hb;
						get_hash(keys[kid],ha,hb);
						if (filled[ha] && filled[hb]) {
							kid = nxt->at(kid);
							continue;
						}
						int helse = filled[ha] ? hb : ha;
						int hthis = filled[ha] ? ha : hb;
						valueType newvalue = values[kid] ^ get(hthis);
						set(helse, newvalue);

						//printf("k%llx ha/hb %llx %llx: %x ^ %x = %x ^ %x = %llx (%llx), helse%llx ,i%llx, %d\n",
						//    keys[kid], ha, hb, get(hthis), newvalue, get(ha), get(hb), get(ha)^get(hb), values[kid], helse ,this ,(bool)(values[kid]==(get(ha)^(get(hb)))));
						Q.push(helse);
						filled[helse] = true;
						kid = nxt->at(kid);
					}
				}
			}
	}

	bool trybuild(keyType *keys, valueType *values, uint32_t keycount) {
		bool succ;
		disj.setLength(ma+mb);
		if (succ = testHash(keys, keycount)) {
			fillvalue(keys, values, keycount);
		}
		delete nxt1;
		delete nxt2;
		delete first;
		disj.finish();
		return succ;
	}
public:
	void inline get_hash_1(const keyType &v, uint32_t &ret1) {
		ret1 = (*Ha)(v);
	}
	void inline get_hash_2(const keyType &v, uint32_t &ret1) {
		ret1 = (*Hb)(v);
		ret1 += ma;
	}
	void inline get_hash(const keyType &v, uint32_t &ret1, uint32_t &ret2) {
		get_hash_1(v,ret1);
		get_hash_2(v,ret2);
	}

	Othello(keyType *_keys, valueType *_values, uint32_t keycount) {
		int hl1 = 0;
		int hl2 = 0;
		while ((1<<hl2) <  keycount * 1) hl2++;
		while ((1<<hl1) < keycount* 1.333334) hl1++;
		Ha = new Hasher32<keyType>(hl1);
		Hb = new Hasher32<keyType>(hl2);
		ma = (1<<hl1);
		mb = (1<<hl2);
		mem.resize(1);
		mem.resize((ma+mb)/(sizeof(mem[0])*8/L));
		cout << "Building Othello " << human(keycount) <<" Keys, ma/mb = " << human(ma) <<"/"<<human(mb) <<" keyType"<< sizeof(keyType)*8<<"b  valueType" << sizeof(valueType)*8<<"b"<<endl;
		while ((!build) && (trycount<MAX_REHASH)) {
			newHash();
			build = trybuild(_keys, _values, keycount);
		}
		cout << "Build Succ "<< build <<" After "<<trycount << "tries"<< endl;
	}

	Othello(vector<keyType> &keys, vector<valueType> &values) :
		Othello(& (keys[0]),& (values[0]),keys.size())
	{
	}
	inline valueType query(const keyType &k) {
		uint32_t ha,hb;
		get_hash_1(k,ha);
		valueType aa = get(ha);
		get_hash_2(k,hb);
		valueType bb = get(hb);
		return aa^bb;
	}


	void printValueTSize() {
		std::cout << sizeof(valueType) << std::endl;
	}

	void exportInfo(void * v) {
		memset(v,0,0x20);
		uint32_t s1 = Ha->s;
		uint32_t s2 = Hb->s;
		memcpy(v,&s1,sizeof(uint32_t));
		memcpy(v+8,&s2,sizeof(uint32_t));
		int hl1 = 0, hl2 = 0;
		while ((1<<hl1)!= ma) hl1++;
		while ((1<<hl2)!= mb) hl2++;
		memcpy(v+0x10,&hl1, sizeof(uint32_t));
		memcpy(v+0x14,&hl2,sizeof(uint32_t));
	}

	Othello(void *v) {
		uint32_t hl1,hl2;
		uint32_t s1,s2;
		memcpy(&(s1),v,sizeof(uint32_t));
		memcpy(&(s2),v+8,sizeof(uint32_t));
		memcpy(&hl1, v+0x10, sizeof(uint32_t));
		memcpy(&hl2, v+0x14, sizeof(uint32_t));
		Ha = new Hasher32<keyType> (hl1);
		Hb = new Hasher32<keyType> (hl2);
		ma = (1<<hl1);
		mb = (1<<hl2);
		mem.resize(1);
		mem.resize((ma+mb)/(sizeof(mem[0])*8/L));
		Ha->setSeed(s1);
		Hb->setSeed(s2);
	}
};






template<uint8_t L, class keyType>
bool Othello<L,keyType>::testHash(keyType *keys, uint32_t keycount) {
	uint32_t ha, hb;
	nxt1  = new vector<int32_t> (keycount);
	nxt2  = new vector<int32_t> (keycount);
	first = new vector<int32_t> (ma+mb, -1);
	disj.clear();
	for (int i = 0; i < keycount; i++) {
		get_hash(keys[i], ha, hb);
		if (disj.sameset(ha,hb)) {
			return false;
		}
		(*nxt1)[i] = (*first)[ha];
		(*first)[ha] = i;
		(*nxt2)[i] = (*first)[hb];
		(*first)[hb] = i;
		disj.merge(ha,hb);
	}
	return true;
}



