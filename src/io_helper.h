#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
template <typename keyT, typename valueT>
bool lineToKVpair(char *s, keyT * k, valueT *v) {
    char *s0; s0 = s;
	switch (*s) {
	case 'A':
	case 'T':
	case 'G':
	case  'C':
		keyT ret = 0;
		while (*s == 'A' || *s == 'C' || *s =='T' || *s =='G') {
			ret <<=2;
			switch (*s) {
			case 'T':
				ret++;
			case 'G':
				ret++;
			case 'C':
				ret++;
			}
			s++;
		}
		*k = ret;
		valueT tv;
		sscanf(s,"%d",&tv);
		*v = tv;
		return true;

	}
	return false;
}


std::string human(uint64_t word) {
	std::stringstream ss;
	if (word <= 1024) ss << word;
	else if (word <= 10240) ss << std::setprecision(2) << word*1.0/1024<<"K";
	else if (word <= 1048576) ss << word/1024<<"K";
	else if (word <= 10485760) ss << word*1.0/1048576<<"M";
	else if (word <= (1048576<<10)) ss << word/1048576<<"M";
	else ss << word*1.0/(1<<30) <<"G";
	std::string s; ss >>s; return s;
}
template<typename keyType>
void splitgrp(const keyType &key, uint32_t &grp, keyType &keyInGroup, uint8_t splitbit) {
    int mvcnt = KMERLENGTH*BIT_PER_BP - splitbit;
    keyType  high = (key >> mvcnt);
    grp = high;
    keyType lowmask = 1;
    lowmask <<= mvcnt;
    keyInGroup = (key & (lowmask-1));
}
