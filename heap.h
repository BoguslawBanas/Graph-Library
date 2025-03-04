#ifndef HEAP_H
#define HEAP_H

#define CONSOLIDATE_SIZE 32
#define BINOMIAL_HEAP_LIST_SIZE 32
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "error_info.h"

template<typename N>
class Heap {
public:
    virtual bool isEmpty() const=0;
    virtual uint32_t getSize() const=0;
    virtual std::pair<uint32_t, N> getMin() const=0;
    virtual uint32_t extractMin()=0;
};

template<typename N>
class BinaryHeap : public Heap<N> {
public:
    struct BinaryHeapNode{
        uint32_t key;
        N value;
        uint32_t position_in_heap;

        BinaryHeapNode(const uint32_t key, const N value, const uint32_t position_in_heap){
            this->key=key;
            this->value=value;
            this->position_in_heap=position_in_heap;
        }
    };
private:
    std::vector<BinaryHeapNode*>heap;

    void swapNodes(const uint32_t node1, const uint32_t node2){
        std::swap(heap[node1], heap[node2]);
        heap[node1]->position_in_heap=node2;
        heap[node2]->position_in_heap=node1;
    }

    void siftUp(uint32_t index){
        int32_t parent_index=(index+1)/2-1;
        while(index!=0 && this->heap[index]->value<this->heap[parent_index]->value){
            this->swapNodes(index, parent_index);
            index=parent_index;
            parent_index=(index+1)/2-1;
        }
    }

    void siftDown(){
        if(this->getSize()<2){
            return;
        }
        uint32_t index=0;
        uint32_t smaller_index;
        while(this->getSize()>=(2*(index+1))){
            if(this->getSize()==2*(index+1) || this->heap[2*index+1]->value<=this->heap[2*index+2]->value){
                smaller_index=2*index+1;
            }
            else{
                smaller_index=2*index+2;
            }

            if(this->heap[smaller_index]->value<this->heap[index]->value){
                this->swapNodes(index, smaller_index);
                index=smaller_index;
            }
            else{
                break;
            }
        }
    }

    void insert(BinaryHeapNode *ptr){
        ptr->position_in_heap=this->heap.size();
        heap.push_back(ptr);
        this->siftUp(ptr->position_in_heap);
    }

public:
    BinaryHeap()=default;

    ~BinaryHeap(){
        for(int32_t i=0;i<this->getSize();++i){
            delete this->heap[i];
        }
    }

    uint32_t getSize() const override {
        return this->heap.size();
    }

    bool isEmpty() const override {
        if(this->getSize()==0){
            return true;
        }
        return false;
    }

    std::pair<uint32_t, N> getMin() const override {
        if(this->isEmpty()){
            printErrorMsg(2, "Unable to get the smallest element from an empty binary heap.");
        }
        return std::pair<uint32_t, N>(this->heap[0]->key, this->heap[0]->value);
    }

    uint32_t extractMin() override {
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to remove the smallest element from an empty binary heap.");
        }
        const uint32_t value_to_return=this->heap[0]->key;
        this->swapNodes(0, this->getSize()-1);

        delete this->heap[this->getSize()-1];
        this->heap.pop_back();
        this->siftDown();

        return value_to_return;
    }

    const BinaryHeapNode* const insert(const uint32_t key, const N &value){
        uint32_t index=this->getSize();
        BinaryHeapNode *ptr=new BinaryHeapNode(key, value, index);
        this->heap.push_back(ptr);
        this->siftUp(index);
        return ptr;
    }

    void decreaseKey(const BinaryHeapNode *ptr, const N new_value){
        if(!ptr){
            printErrorMsg(2, "DecreaseKey method in a class that represents binary heap received a pointer to NULL.");
        }
        uint32_t index=ptr->position_in_heap;
        heap[index]->new_value=new_value;
        this->siftUp(index);
    }

    void unionize(BinaryHeap *heap_to_union){
        uint32_t size_heap_to_union=heap_to_union->heap.size();
        for(int i=0;i<size_heap_to_union;++i){
            this->insert(heap_to_union->heap[i]);
        }
    }
};

// template<typename N>
// class BinomialHeap : public Heap<N> {
// public:
//     struct BinomialHeapNode{
//         uint32_t key;
//         N key_value;
//         uint32_t degree;
//         BinomialHeapNode *child;
//         BinomialHeapNode *sibling;
//         BinomialHeapNode *parent;

//         BinomialHeapNode(const uint32_t key, const N value){
//             this->key=key;
//             this->key_value=value;
//             this->degree=0;
//             this->child=this->sibling=this->parent=nullptr;
//         }
//     };
// private:
//     BinomialHeapNode *head;
//     uint32_t size;

//     BinomialHeapNode* find_min() const{
//         if(!this->head){
//             return nullptr;
//         }
//         BinomialHeapNode *result=this->head;
//         BinomialHeapNode *ptr=this->head->sibling;

//         while(ptr){
//             if(ptr->key_value<result->key_value) result=ptr;
//             ptr=ptr->sibling;
//         }
//         return result;
//     }

//     void priv_union(){
//         BinomialHeapNode *ptr=this->head;
//         BinomialHeapNode *prev=nullptr;
//         BinomialHeapNode *nxt=this->head->sibling;
//         while(nxt!=nullptr){
//             if(ptr->degree!=nxt->degree){
//                 prev=ptr;
//                 ptr=ptr->sibling;
//                 nxt=ptr->sibling;
//             }
//             else if(ptr->sibling->sibling!=nullptr && ptr->degree==nxt->degree && nxt->degree==nxt->sibling->degree){
//                 prev=ptr;
//                 ptr=ptr->sibling;
//                 nxt=ptr->sibling;
//             }
//             else if(ptr->degree==nxt->degree && ptr->key_value<=nxt->key_value){
//                 ptr->sibling=nxt->sibling;
//                 nxt->sibling=ptr->child;
//                 ptr->child=nxt;
//                 nxt->parent=ptr;
//                 nxt=ptr->sibling;
//                 ++ptr->degree;
//             }
//             else{
//                 if(prev==nullptr) this->head=nxt;
//                 else prev->sibling=nxt;
//                 ptr->sibling=nxt->child;
//                 nxt->child=ptr;
//                 ptr->parent=nxt;
//                 ++nxt->degree;
//                 ptr=nxt;
//                 nxt=ptr->sibling;
//             }
//         }
//     }

//     void priv_dfs(BinomialHeapNode *ptr){
//         if(ptr==nullptr) return;
//         BinomialHeapNode *ptr2=nullptr;
//         do{
//             ptr2=ptr->sibling;
//             priv_dfs(ptr->child);
//             delete ptr;
//             ptr=ptr2;
//         }while(ptr!=nullptr);
//     }

// public:
//     BinomialHeap() {
//         this->size=0;
//         head=nullptr;
//     }

//     ~BinomialHeap(){
//         priv_dfs(this->head);
//     }

//     bool isEmpty() const override {
//         return this->size==0;
//     }

//     uint32_t getSize() const override {
//         return this->size;
//     }

//     const BinomialHeapNode* const insert(const uint32_t node, const N value) {
//         BinomialHeapNode *ptr=new BinomialHeapNode(node,value);
//         if(this->isEmpty()){
//             this->head=ptr;
//         }
//         else{
//             ptr->sibling=this->head;
//             this->head=ptr;
//             priv_union();
//         }
//         ++this->size;
//         return ptr;
//     }

//     std::pair<uint32_t, N> getMin() const override{
//         if(this->isEmpty()){
//             printErrorMsg(2, "Trying to get the smallest element from an empty binomial heap.");
//         }
//         BinomialHeapNode *min=this->find_min();
//         return std::pair<uint32_t, N>(min->key, min->key_value);
//     }

//     uint32_t extractMin() override{
//         if(this->isEmpty()){
//             printErrorMsg(2, "Trying to extract the smallest element from an empty binomial heap.");
//         }
//         BinomialHeapNode *min=this->find_min();
//         const uint32_t min_key=min->key;

//         if(this->getSize()==1){
//             delete this->head;
//             this->head=nullptr;
//             this->size=0;
//             return min_key;
//         }

//         BinomialHeapNode *ptr=this->head;
//         BinomialHeapNode *children_ptr=min->child;

//         std::vector<BinomialHeapNode*>tmp_children;
//         std::vector<BinomialHeapNode*>tmp_heads;
//         std::vector<BinomialHeapNode*>new_list;

//         while(children_ptr!=nullptr){
//             tmp_children.push_back(children_ptr);
//             children_ptr=children_ptr->sibling;
//         }

//         while(ptr!=nullptr){
//             if(ptr!=min){
//                 tmp_heads.push_back(ptr);
//             }
//             ptr=ptr->sibling;
//         }

//         delete min;

//         int32_t i=tmp_children.size()-1;
//         int32_t j=0;

//         while(i>=0 && j<tmp_heads.size()){
//             if(tmp_heads[j]->degree<tmp_children[i]->degree){
//                 new_list.push_back(tmp_heads[j]);
//                 ++j;
//             }
//             else{
//                 new_list.push_back(tmp_children[i]);
//                 --i;
//             }
//         }
//         while(i>=0){
//             new_list.push_back(tmp_children[i]);
//             --i;
//         }
//         while(j<tmp_heads.size()){
//             new_list.push_back(tmp_heads[j]);
//             ++j;
//         }

//         this->head=min=new_list[0];
//         new_list.back()->sibling=nullptr;
//         for(uint32_t k=1;k<new_list.size();++k){
//             new_list[k-1]->sibling=new_list[k];
//             if(min->key_value>new_list[k]->key_value){
//                 min=new_list[k];
//             }
//         }
//         priv_union();
//         ptr=this->head;
//         while(ptr){
//             ptr->parent=nullptr;
//             ptr=ptr->sibling;
//         }
//         --this->size;
//         return min_key;
//     }

//     void decreaseKey(BinomialHeapNode *ptr, const N new_value, BinomialHeapNode *container) {
//         if(new_value>ptr->key_value){
//             printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
//         }
//         ptr->key_value=new_value;
//         while(ptr->parent!=nullptr && ptr->key_value<ptr->parent->key_value){
//             if(container){
//                 std::swap(container[ptr->key], container[ptr->parent->key]);
//             }
//             std::swap(ptr->key, ptr->parent->key);
//             std::swap(ptr->key_value, ptr->parent->key_value);
//             ptr=ptr->parent;
//         }
//     }

//     void unionize(BinomialHeap *to_union) {
//         if(to_union->isEmpty()){
//             delete to_union;
//             return;
//         }

//         BinomialHeapNode *new_min=nullptr;
//         if(to_union->getMin().second<this->getMin().second){
//             new_min=to_union->getMin();
//         }
//         else{
//             new_min=this->getMin();
//         }
//         BinomialHeapNode *tmp=head;
//         BinomialHeapNode *tmp2=to_union->head;
//         BinomialHeapNode *tmp3=nullptr;
//         BinomialHeapNode *new_head=nullptr;

//         if(tmp->degree<=tmp2->degree){
//             tmp3=tmp;
//             tmp=tmp->sibling;
//         }
//         else{
//             tmp3=tmp2;
//             tmp2=tmp2->sibling;
//         }
//         new_head=tmp3;
//         while(tmp!=nullptr && tmp2!=nullptr){
//             if(tmp->degree<=tmp2->degree){
//                 tmp3->sibling=tmp;
//                 tmp=tmp->sibling;
//             }
//             else{
//                 tmp3->sibling=tmp2;
//                 tmp2=tmp2->sibling;
//             }
//             tmp3=tmp3->sibling;
//         }
//         while(tmp!=nullptr){
//             tmp3->sibling=tmp;
//             tmp=tmp->sibling;
//             tmp3=tmp3->sibling;
//         }
//         while(tmp2!=nullptr){
//             tmp3->sibling=tmp2;
//             tmp2=tmp2->sibling;
//             tmp3=tmp3->sibling;
//         }
//         this->size+=to_union->getSize();
//         this->head=new_head;
//         this->priv_union();
//         this->min=new_min;
//         to_union->head=nullptr;
//         delete to_union;
//     }
// };

template<typename N>
class BinomialHeap : public Heap<N> {
public:
    struct BinomialHeapNode{
        uint32_t key;
        N value;
        BinomialHeapNode *parent;
        BinomialHeapNode *left_s;
        BinomialHeapNode *right_s;
        BinomialHeapNode *child;
        uint32_t degree;

        BinomialHeapNode(const uint32_t key, const N value){
            this->key=key;
            this->value=value;
            this->degree=0;
            this->parent=this->left_s=this->right_s=this->child=nullptr;
        }
    };

private:
    uint32_t size;
    BinomialHeapNode *head;

    void dfs_delete(){

    }

public:
    BinomialHeap(){
        this->size=0;
        this->head=nullptr;
    }

    ~BinomialHeap(){
        this->dfs_delete();
    }

    bool isEmpty() const override {

    }

    uint32_t getSize() const override {

    }

    std::pair<uint32_t, N> getMin() const override {

    }

    uint32_t extractMin() override {

    }

    const BinomialHeapNode* const insert(const uint32_t key, const N value) {

    }

    void decreaseKey() {

    }

    void unionize() {

    }
};


template<typename N>
class FibonacciHeap : public Heap<N> {
public:
    struct FibonacciHeapNode{
        uint32_t key;
        N value;
        FibonacciHeapNode *child;
        FibonacciHeapNode *left_s;
        FibonacciHeapNode *right_s;
        FibonacciHeapNode *parent;
        uint32_t degree;
        bool mark;

        FibonacciHeapNode(const uint32_t key, const N value){
            this->key=key;
            this->value=value;
            this->child=this->left_s=this->right_s=this->parent=nullptr;
            this->degree=0;
            this->mark=false;
        }
    };
    
private:
    FibonacciHeapNode *min;
    uint32_t size;

    void addToList(FibonacciHeapNode *ptr, FibonacciHeapNode *listNode){
        if(listNode==nullptr){
            listNode=ptr;
            ptr->left_s=ptr->right_s=ptr;
            return;
        }
        else{
            FibonacciHeapNode *tmp=listNode->right_s;
            listNode->right_s=ptr;
            ptr->left_s=listNode;
            ptr->right_s=tmp;
            tmp->left_s=ptr;
        }
    }

    void consolidate(FibonacciHeapNode *ptr, std::vector<FibonacciHeapNode*>&consolidate_arr){
        const uint32_t d=ptr->degree;
        if(!consolidate_arr[d]){
            consolidate_arr[d]=ptr;
        }
        else{
            FibonacciHeapNode *ptr2=consolidate_arr[d];
            consolidate_arr[d]=nullptr;
            if(ptr->value<ptr2->value){
                ptr2->parent=ptr;
                addToList(ptr2, ptr->child);
                ptr->child=ptr2;
                ++ptr->degree;
                consolidate(ptr, consolidate_arr);
            }
            else{
                ptr->parent=ptr2;
                addToList(ptr, ptr2->child);
                ptr2->child=ptr;
                ++ptr2->degree;
                consolidate(ptr2, consolidate_arr);
            }
        }
    }

    void cutNode(FibonacciHeapNode *node){
        if(node->parent==nullptr) return;
        FibonacciHeapNode *p=node->parent;
        node->parent=nullptr;
        if(node->left_s==node) p->child=nullptr;
        else{
            p->child=node->left_s;
            FibonacciHeapNode *l=node->left_s;
            FibonacciHeapNode *r=node->right_s;
            l->right_s=r;
            r->left_s=l;
        }
        --p->degree;
        this->addToList(node, min);
        node->mark=false;
    }

    void cascadingCutNode(FibonacciHeapNode *node){
        if(node->parent==nullptr) return;
        FibonacciHeapNode *p=node->parent;
        if(node->mark==false) node->mark=true;
        else{
            this->cutNode(node);
            this->cascadingCutNode(p);
        }
    }

    void priv_dfs(FibonacciHeapNode *node){
        if(node==nullptr) return;
        FibonacciHeapNode *ptr=node;
        FibonacciHeapNode *ptr2=nullptr;
        do{
            ptr2=ptr->right_s;
            priv_dfs(ptr->child);
            delete ptr;
            ptr=ptr2;
        }while(ptr!=node);
    }

public:
    FibonacciHeap() {
        this->min=nullptr;
        size=0;
    }

    ~FibonacciHeap(){
        priv_dfs(this->min);
    }

    bool isEmpty() const override {
        if(this->getSize()==0) return true;
        return false;
    }

    uint32_t getSize() const override {
        return this->size;
    }

    const FibonacciHeapNode* const insert(const uint32_t node, const N value) {
        FibonacciHeapNode *ptr=new FibonacciHeapNode(node,value);
        addToList(ptr,this->min);
        if(this->min==nullptr || this->min->value>value) this->min=ptr;
        ++this->size;
        return ptr;
    }

    std::pair<uint32_t,N> getMin() const override {
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to get the smallest element from an empty fibonacci heap.");
        }
        return std::pair<uint32_t,N> (this->min->key, this->min->value);
    }

    uint32_t extractMin() override{
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to extract the smallest element from an empty fibonacci heap.");
        }

        const uint32_t return_key=this->min->key;
        if(this->size==1){
            delete this->min;
            this->min=nullptr;
            this->size=0;
            return return_key;
        } 

        FibonacciHeapNode *p=this->min->child;
        std::vector<FibonacciHeapNode*>list_of_children;
        if(p!=nullptr){
            do{
                list_of_children.push_back(p);
                p=p->right_s;
            }while(p!=this->min->child);
        }
        for(FibonacciHeapNode *it : list_of_children){
            addToList(it, this->min);
        }

        p=this->min->right_s;
        this->min->left_s->right_s=this->min->right_s;
        this->min->right_s->left_s=this->min->left_s;
        delete min;
        this->min=p;

        std::vector<FibonacciHeapNode*>consolidate_arr(CONSOLIDATE_SIZE, nullptr);
        std::vector<FibonacciHeapNode*>tmp_arr;
        FibonacciHeapNode *it=this->min;
        do{
            tmp_arr.push_back(it);
            it=it->right_s;
        }while(it!=this->min);
        
        for(FibonacciHeapNode* i : tmp_arr){
            consolidate(i, consolidate_arr);
        }

        this->min=nullptr;

        for(uint32_t i=0;i<CONSOLIDATE_SIZE;++i){
            if(consolidate_arr[i]){
                FibonacciHeapNode *ptr=consolidate_arr[i];
                if(this->min==nullptr){
                    addToList(ptr, this->min);
                    this->min=ptr;
                }
                else{
                    addToList(ptr, this->min);
                    if(ptr->value<this->min->value) this->min=ptr;
                }
            }
        }
        --this->size;
    }

    void decreaseKey(FibonacciHeapNode *ptr, const N new_value) {
        if(new_value>ptr->value){
            printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
        }
        ptr->value=new_value;
        if(ptr->parent!=nullptr && new_value<ptr->parent->value){
            FibonacciHeapNode *p=ptr->parent;
            cutNode(ptr);
            cascadingCutNode(p);
        }
        if(this->min->value>new_value) this->min=ptr;
    }

    void unionize(FibonacciHeap *to_union) {
        if(to_union->isEmpty()){
            delete to_union;
            return;
        }

        FibonacciHeapNode *minPtr=to_union->min;
        FibonacciHeapNode *tmp1=to_union->min->left_s;
        FibonacciHeapNode *rs=this->min->right_s;
        to_union->min->left_s=this->min;
        this->min->right_s=to_union->min;
        tmp1->right_s=rs;
        rs->left_s=tmp1;
        if(minPtr->value<this->min->value){
            this->min=to_union->min;
        }
        this->size+=to_union->getSize();
        to_union->min=nullptr;
        delete to_union;
    }
};

#endif
