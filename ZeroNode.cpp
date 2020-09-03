#include "ZeroNode.h"
#include "Perm.h"

void ZeroNode::enter(){
        swapMain(p);
        sum = computeSum(*vmain);
        Perm *perm = (Perm*)owner;
    }

void ZeroNode::leave(){
        swapMain(p);
        Perm *perm = (Perm*)owner;
    }


ZeroNode::ZeroNode(pair<int,int> p, void *owner, ZeroNode* parent, int depth):
        p(std::move(p)),owner(owner),parent(parent),depth(depth) {
        vmain = ((Perm*)owner)->vmain;
    }