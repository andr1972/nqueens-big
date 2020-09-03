#pragma once
#include <utility>
#include <vector>
#include "scores.h"
#include "checksum.h"
#include "ThreadData.h"

using namespace std;

class ZeroNode {
    vector<ZeroNode*> childs;
    int childPos = 0;
    vector<int> *vmain;
    void *owner;
    void swapMain(pair<int,int> &p) {
        swapTab(*vmain,p.first,p.second);
    }
    void enter();
    void leave();

    ZeroNode *getNextChild() {
        if (childPos<childs.size()) {
            auto child =  childs[childPos];
            childPos++;
            return child;
        }
        else return nullptr;
    }

public:
    explicit ZeroNode (pair<int,int> p, void *owner, ZeroNode* parent, int depth);
    ~ZeroNode(){
        for (auto child: childs) delete child;
    }
    int depth;
    ZeroNode* parent= nullptr;
    uint64_t sum=0;
    pair<int,int> p;
    void addChilds(NewInterchangeData *chgData) {
        for (auto &p: chgData->zeroTransitions) {
            auto child = new ZeroNode(p, owner, this, depth+1);
            childs.push_back(child);
        }
        chgData->zeroTransitions.clear();
    }

    ZeroNode *goFirstChild() {
        if (childs.empty()) return nullptr;
        childs[0]->enter();
        return childs[0];
    }

    ZeroNode *goNextSiblingOrUncle() {
        auto descendant = this;
        auto ancestor = parent;
        while (ancestor){
            descendant->leave();
            auto siblingOrUncle = ancestor->getNextChild();
            if (siblingOrUncle) {
                siblingOrUncle->enter();
                return siblingOrUncle;
            }
            descendant = ancestor;
            ancestor = ancestor->parent;
        }
        return nullptr;
    }
};