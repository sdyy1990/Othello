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
#include "typedefine.h"
public:
	vector<valueType> mem;
	uint32_t ma;
	uint32_t mb;
	Hasher32<keyType> Ha;
	Hasher32<keyType> Hb;
	bool build = false;
	uint32_t trycount = 0;
	DisjointSet disj;

	template<class VT = valueType>
	inline typename std::enable_if< sizeof(valueType)*8 == L, VT>::type
	get(uint32_t loc) { // 0..m_a-1 : arra, m_a..m_a+m_b-1 : arrb;
			return mem[loc];
    }

	template<class VT = valueType>
	inline typename std::enable_if< !(sizeof(valueType)*8 == L), VT>::type
    get(uint32_t loc){
			//partial;  e.g, 33221100
			const uint32_t percell = sizeof(mem[0])*8/L;
			return (mem[loc/percell] >> ((loc % percell)*L))&((1<<L)-1);
	}
    
	
	template<class VT = valueType>
	inline typename std::enable_if< sizeof(valueType)*8 == L, VT>::type
    set(uint32_t loc, valueType &value) {
            return mem[loc] = value;
    }
	
    template<class VT = valueType>
	inline typename std::enable_if< !(sizeof(valueType)*8 == L), VT>::type
    set(uint32_t loc, valueType &value) {
			//partial;
            value &= ((1<<L)-1);
			const uint32_t percell = sizeof(mem[0])*8/L;
			uint32_t mv = loc % percell;
			// 33221100--> 3322xx00, changedbits = 11^xx
			valueType mask1 = ~(((1<<L)-1) << (mv*L));
			mem[loc/percell] &= mask1;
			mem[loc/percell] ^= (value << (mv*L));
            return value;
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
	    std::is_same<VT, uint16_t>::value ||  std::is_same<VT, uint8_t>::value
	    , VT>::type
	getrand(valueType &v) {
//		v = rand();
		v = 0;
	}


	void newHash() {
		Ha.setMaskSeed(ma-1,rand());
		Hb.setMaskSeed(mb-1,rand());
		trycount++;
	}

	vector<int32_t> *first, *nxt1, *nxt2;
	bool testHash(keyType *keys, uint32_t keycount);

	void fillvalue(keyType *keys, valueType *values, uint32_t keycount);
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
		ret1 = (Ha)(v);
	}
	void inline get_hash_2(const keyType &v, uint32_t &ret1) {
		ret1 = (Hb)(v);
		ret1 += ma;
	}
	void inline get_hash(const keyType &v, uint32_t &ret1, uint32_t &ret2) {
		get_hash_1(v,ret1);
		get_hash_2(v,ret2);
	}

	Othello(keyType *_keys, valueType *_values, uint32_t keycount) {
		int hl1 = 2;
		int hl2 = 2;
		while ((1<<hl2) <  keycount * 1) hl2++;
		while ((1<<hl1) < keycount* 1.333334) hl1++;
		ma = (1<<hl1);
		mb = (1<<hl2);
		mem.resize(1);
		mem.resize((ma+mb)/(sizeof(mem[0])*8/L));
        cout << "Othello" << human(keycount) <<" Keys, ma/mb = " << human(ma) <<"/"<<human(mb) <<" keyT"<< sizeof(keyType)*8<<"b  valueT" << sizeof(valueType)*8<<"b"<<" L="<<(int) L<<endl;
		while ((!build) && (trycount<MAX_REHASH)) {
			newHash();
			build = trybuild(_keys, _values, keycount);
		}
		cout << "Build Succ "<< build <<" After "<<trycount << "tries"<< endl;
	}

	Othello(vector<keyType> &keys, vector<valueType> &values) :
		Othello(& (keys[0]),& (values[0]),keys.size())
	{}

	inline valueType query(const keyType &k) {
		uint32_t ha,hb;
		get_hash_1(k,ha);
		valueType aa = get(ha);
		get_hash_2(k,hb);
		valueType bb = get(hb);
        //printf("%llx   [%x] %x ^ [%x] %x = %x\n", k,ha,aa,hb,bb,aa^bb);
		return aa^bb;
	}
	
    template<class VT = valueIntType>
	inline typename std::enable_if< !std::is_same<VT,valueType>::value, VT>::type
    queryInt(const keyType &k) {
        return 0;
    }

    template<class VT = valueIntType>
	inline typename std::enable_if< std::is_same<VT,valueType>::value, VT>::type
    queryInt(const keyType &k) {
        return query(k);
    }


	void printValueTSize() {
		std::cout << sizeof(valueType) << std::endl;
	}

	void exportInfo(void * v) {
		memset(v,0,0x20);
		uint32_t s1 = Ha.s;
		uint32_t s2 = Hb.s;
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
		ma = (1<<hl1);
		mb = (1<<hl2);
		mem.resize(1);
		mem.resize((ma+mb)/(sizeof(mem[0])*8/L));
		Ha.setMaskSeed(ma-1,s1);
		Hb.setMaskSeed(mb-1,s2);
	}
    array<uint32_t, L*2> getCnt(); // returns an array (length of 2L), postion x: the number of 1s on the x-th lowest bit, for array A, if x<L; otherwise, for arrayB.
    array<double, L> getRatio(); // returns an array, postion x: the probability that query return 1 on the x-th lowest bit.


};

template<size_t L>
std::array<uint8_t,L> operator ^ (const std::array<uint8_t,L>  &A,const std::array<uint8_t,L>  &B) {
    std::array<uint8_t, L> v = A;
    for (int i = 0 ; i < L; i++) v[i]^=B[i];
    return v;
    
}



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


template<uint8_t L, class keyType>
void Othello<L,keyType>::fillvalue(keyType *keys, valueType *values, uint32_t keycount) {
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
						//printf("k%llx ha/hb %lx %lx: %x ^ %x = %x ^ %x = %x (%x), helse%x ,i%x, %d\n",
						//    keys[kid], ha, hb, get(hthis), newvalue, get(ha), get(hb), get(ha)^get(hb), values[kid], helse ,hthis ,(bool)(values[kid]==(get(ha)^(get(hb)))));
						Q.push(helse);
						filled[helse] = true;
						kid = nxt->at(kid);
					}
				}
			}
	}

template<uint8_t L, class keyType>
array<uint32_t, L*2> Othello<L,keyType>::getCnt() {
    array<uint32_t, L*2> cnt;
    for (int i = 0 ; i < L+L; i++) cnt[i] =0;
    for (int i = 0 ; i < ma; i++) {
        valueType gv = get(i);
        uint8_t *vv; vv = (uint8_t*) ((void *) &gv);
        for (int p = 0 ; p < L; p++)  {
            uint8_t tv; tv =  ((*vv) >> (p & 7));
            if (tv & 1) cnt[p] ++;
            if ((p & 7) == 7) vv++;
        }
    }
    for (int i = ma ; i < ma+mb; i++) {
        valueType gv = get(i);
        uint8_t *vv; vv = (uint8_t*) ((void *) &gv);
        for (int p = 0 ; p < L; p++)  {
            uint8_t tv; tv =  ((*vv) >> (p & 7));
            if (tv & 1) cnt[L+p] ++;
            if ((p & 7) == 7) vv++;
        }
    }
    return cnt;
}


template<uint8_t L, class keyType>
array<double, L> Othello<L,keyType>::getRatio() {
    array<uint32_t, 2*L> cnt = getCnt();
    array<double, L> ret;
    for (int i = 0 ; i < L; i++) {
        double p1 = 1.0 * cnt[i] / ma;
        double p2 = 1.0 * cnt[i+L] / mb;
        ret[i] = p1*(1-p2)+p2*(1-p1);
    }
    return ret;
        
}
