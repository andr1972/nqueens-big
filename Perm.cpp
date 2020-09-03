#include "Perm.h"
#include <fstream>

void Perm::dumpPair(ofstream &ofs,pair<int,int> &p) {
    ofs << p.first+1 << "," << p.second+1;
}

void Perm::dumpStruct(ofstream &ofs) {
    ZeroNode *current = (ZeroNode *)xchgData.current;
    auto node = current;
    while (node) {
        ofs << "depth=" << node->depth << endl;
        ofs << "parentPaint=";
        dumpPair(ofs, node->p);
        ofs << endl;
        int trCount = xchgData.zeroTransitions.size();
        ofs << "zeroTr=";
        for (int i=0; i<trCount; i++)
        {
            if (i>0) ofs << " ";
            dumpPair(ofs, xchgData.zeroTransitions[i]);
        }
        ofs << endl;
        node = node->parent;
    }
}

bool Perm::tryWorse(int worstDiff) {
    xchgData.worstDiff = worstDiff;
    startThreads(nullptr, true);
    stopThreads(true);
    bool b = xchgData.ret_dcnt<INT32_MAX;
    if (b) {
        swapTab(*vmain, xchgData.ret_p.first, xchgData.ret_p.second);
    }
    return b;
}
