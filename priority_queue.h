#ifndef PQ_H
#define PQ_H
#include <vector>

template<typename H, typename N>
class Priority_Q {
private:
    H *h=nullptr;
    uint32_t max_size;
    std::vector<void*>v;
public:
    Priority_Q(uint32_t maximum_size) {
        h=new H();
        this->max_size=maximum_size;
        v=std::vector<void*>(maximum_size, nullptr);
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
        if(v[node]){
            h->decreaseKey(v[node], value);
        }
        else{
            void *ptr=h->insert(node, value);
            v[node]=ptr;
        }
    }

    void pop() {
        h->extractMin();
    }
};

#endif
