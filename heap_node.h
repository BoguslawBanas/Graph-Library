#ifndef HEAP_NODE_H
#define HEAP_NODE_H

#include <cstdint>

class HeapNode {
    private:
        void *ptr;
    public:
        HeapNode(){
            this->ptr=nullptr;
        }
    
        HeapNode(void *ptr){
            this->ptr=ptr;
        }
    
        ~HeapNode()=default;
    
        void *getPtr(){
            return this->ptr;
        }
    };

#endif