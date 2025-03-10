#ifndef PQ_H
#define PQ_H

template<typename H, typename N>
class Priority_Q {
private:
    H *h=nullptr;
public:
    Priority_Q() {
        h=new H();
    }

    ~Priority_Q(){
        delete h;
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

    void push(const uint32_t node, const N value, const bool doesContain) {
        if(doesContain){
            h->decreaseKey(node,value);
        }
        else{
            h->insert(node, value);
        }
    }

    void pop() {
        h->extractMin();
    }
};

#endif
