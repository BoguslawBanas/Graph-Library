#ifndef PQ_H
#define PQ_H
#include <vector>
#include "heap_node.h"

template<typename H, typename N>
class Priority_Q {
private:
    H *h=nullptr;
    uint32_t max_size;
    std::vector<HeapNode>v;
public:
    Priority_Q(uint32_t maximum_size) {
        h=new H();
        this->max_size=maximum_size;
        v=std::vector<HeapNode>(maximum_size, HeapNode());
    }

    ~Priority_Q(){
        delete h;
        v.clear();
    };

    std::pair<uint32_t, N> top(){
        return h->getMin();
    }

    bool empty() {
        return h->isEmpty();
    }

    uint32_t size() {
        return h->getSize();
    }

    void push(const uint32_t node, const N value) {
        if(v[node].getPtr()){
            h->decreaseKey(&v[node], value);
        }
        else{
            v[node]=h->insert(node, value);
        }
    }

    void pop() {
        uint32_t index=h->extractMin();
        v[index]=HeapNode(nullptr, index);
    }
};

#endif
