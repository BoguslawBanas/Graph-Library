#ifndef HEAP_NODE_H
#define HEAP_NODE_H

#include <cstdint>

class HeapNode {
    private:
        void *ptr;
        uint32_t key;
    public:
        HeapNode(){
            this->ptr=nullptr;
            this->key=1<<31;
        }
    
        HeapNode(void *ptr, uint32_t key){
            this->ptr=ptr;
            this->key=key;
        }
    
        ~HeapNode()=default;
    
        void *getPtr(){
            return this->ptr;
        }
    
        uint32_t getKey(){
            return this->key;
        }
    };

#endif