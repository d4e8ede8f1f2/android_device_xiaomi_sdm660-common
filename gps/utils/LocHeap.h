
#ifndef __LOC_HEAP__
#define __LOC_HEAP__

#include <stddef.h>
#include <string.h>

// abstract class to be implemented by client to provide a rankable class
class LocRankable {
public:
    virtual inline ~LocRankable() {}

    // method to rank objects of such type for sorting purposes.
    // The pointer of the input node would be stored in the heap.
    // >0 if ranks higher than the input;
    // ==0 if equally ranks with the input;
    // <0 if ranks lower than the input
    virtual int ranks(LocRankable& rankable) = 0;

    // convenient method to rank objects of such type for sorting purposes.
    inline bool outRanks(LocRankable& rankable) { return ranks(rankable) > 0; }
};

// opaque class to provide service implementation.
class LocHeapNode;

// a heap whose left and right children are not sorted. It is sorted only vertically,
// i.e. parent always ranks higher than children, if they exist. Ranking algorithm is
// implemented in Rankable. The reason that there is no sort between children is to
// help beter balance the tree with lower cost. When a node is pushed to the tree,
// it is guaranteed that the subtree that is smaller gets to have the new node.
class LocHeap {
protected:
    LocHeapNode* mTree;
public:
    inline LocHeap() : mTree(NULL) {}
    ~LocHeap();

    // push keeps the tree sorted by rank, it also tries to balance the
    // tree by adding the new node to the smaller of the subtrees.
    // node is reference to an obj that is managed by client, that client
    //      creates and destroyes. The destroy should happen after the
    //      node is popped out from the heap.
    void push(LocRankable& node);

    // Peeks the node data on tree top, which has currently the highest ranking
    // There is no change the tree structure with this operation
    // Returns NULL if the tree is empty, otherwise pointer to the node data of
    //         the tree top.
    LocRankable* peek();

    // pop keeps the tree sorted by rank, but it does not try to balance
    // the tree.
    // Return - pointer to the node popped out, or NULL if heap is already empty
    LocRankable* pop();

    // navigating through the tree and find the node that ranks the same
    // as the input data, then remove it from the tree. Rank is implemented
    // by rankable obj.
    // returns the pointer to the node removed; or NULL (if failed).
    LocRankable* remove(LocRankable& rankable);

#ifdef __LOC_UNIT_TEST__
    bool checkTree();
    uint32_t getTreeSize();
#endif
};

#endif //__LOC_HEAP__
