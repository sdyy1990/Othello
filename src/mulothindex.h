#pragma once
#include "othelloindex.h"
/*!
 \file mulothindex.h
 Describes the data structure *MulOthIndex*

 */

/*!
 \breif MulOthIndex is a extended version of OthelloIndex, it supports more than 2^29 keys. \n
  The keys are first classifed into groups, each group is then maintained by one *OthelloIndex*.
  \note Query a key of keyType always return uint64_t, however, only the lowest bits are meaningful.
*/

template <class keyType>
class MulOthIndex {
    typedef uint64_t valueType;
    vector<OthelloIndex<keyType> *> vOthIdxs;
    vector<valueType> shift;
    unsigned char split;
    bool addOthIndex(unsigned int groupid, vector<keyType> &keys) {
        OthelloIndex<keyType> *poth;
        poth = new OthelloIndex<keyType>(&keys[0], keys.size());
        if (!poth->build) {
            printf("Build Halt!\n");
            return false;
        }
        vOthIdxs[groupid] = poth;
        shift[groupid+1] = shift[groupid] + keys.size();
        return true;
    }
    IOHelper<keyType,valueType> *helper;
public:
    bool buildsucc;
    MulOthIndex(const char * fname, unsigned char _split, class IOHelper<keyType,valueType> * _helper, bool fileIsSorted = false) : helper(_helper) {
        printf("Building MulOthelloIndex from file %s\n", fname);
        FILE *pFile;
        pFile = fopen (fname, "r");
        vOthIdxs. clear();
        vOthIdxs.resize(1<<_split);
        shift. clear();
        shift.resize(1<<_split);
        char buf[1024];
        if (split ==0) {
            keyType k;
            vector<keyType> keys;
            while (true) {
                if (fgets(buf,1024,pFile)==NULL) break;
                if (!helper->convert(buf,&k)) break;
                keys.push_back(k);
            }
            buildsucc = addOth(0,keys);
            fclose(pFile);
            return;
        }
        if (fileIsSorted) {
            uint32_t grpid = 0;
            printf("Reading file for keys in group %2x/%2x\n", grpid,(1<<split)-1);
            vector<keyType> keys;
            while (true) {
                keyType k;
                void * pp;
                if (fgets(buf,1024,pFile)==NULL) break;
                if (!helper->convert(buf, &k)) break;
                keyType keyingroup;
                uint32_t groupid;
                helper->splitgrp(k,groupid,keyingroup);
                if (groupid != grpid) {
                    if (!addOthIndex(grpid,keys))
                    {
                        fclose(pFile);
                        return;
                    }
                    grpid = groupid;
                    printf("Reading file for keys in group %2x/%0x\n", grpid,(1<<split)-1);
                    keys.clear();
                }
                keys.push_back(keyingroup);
            }
            if (!addOth(grpid,keys))
            {
                fclose(pFile);
                return;
            }
        }
        else {
            for (uint32_t grpid = 0; grpid < (1<<_split); grpid++) {
                printf("Reading file for keys in group %2x/%2x\n", grpid,(1<<split)-1);
                vector<keyType> keys;
                rewind(pFile);
                while (true) {
                    keyType k;
                    if (fgets(buf,1024,pFile)==NULL) break;
                    if (!helper->convert(buf, &k)) break;
                    keyType keyingroup;
                    uint32_t groupid;
                    helper->splitgrp(k,groupid,keyingroup);
                    if (groupid != grpid) continue;
                    keys.push_back(keyingroup);
                }
                printf("keycount %d ", keys.size());
                if (keys.size()>0)
                    if (!addOth(grpid,keys)) {
                        fclose(pFile);
                        return;
                    }

            }
        }
        buildsucc = true;
        fclose(pFile);
    }

    inline valueType query(const keyType &k) {
        uint32_t grp;
        keyType kgrp;
        if (split ==0)
            return vOthIdxs[0]->query(k);
        else {
            helper->splitgrp(k,grp,kgrp);
            return (vOthIdxs[grp]!=NULL)?shift[grp]+(vOthIdxs[grp]->query(kgrp)):0;
        }
    }
};
