#ifndef HEAP_H
#define HEAP_H

#define CONSOLIDATE_SIZE 32
#define BINOMIAL_HEAP_LIST_SIZE 32
#include <vector>
#include <algorithm>
#include <cstdint>
#include "heap_node.h"
#include "error_info.h"

template<typename N>
class Heap {
public:
    virtual bool isEmpty() const=0;
    virtual uint32_t getSize() const=0;
    virtual std::pair<uint32_t, N> getMin() const=0;
    virtual uint32_t extractMin()=0;
    virtual HeapNode insert(const uint32_t, const N)=0;
    virtual void decreaseKey(HeapNode*, const N)=0;
};

template<typename N>
class BinaryHeap : public Heap<N> {
private:
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

    HeapNode insert(const uint32_t key, const N value) override{
        uint32_t index=this->getSize();
        BinaryHeapNode *ptr=new BinaryHeapNode(key, value, index);
        this->heap.push_back(ptr);
        this->siftUp(index);
        return HeapNode((void*)ptr, key);
    }

    void decreaseKey(HeapNode *heap_node, const N new_value) override{
        if(!heap_node->getPtr()){
            printErrorMsg(2, "DecreaseKey method in a class that represents binary heap received a pointer to NULL.");
        }
        BinaryHeapNode *ptr=(BinaryHeapNode*)heap_node->getPtr();
        if(ptr->value<new_value){
            printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
        }
        uint32_t index=ptr->position_in_heap;
        heap[index]->value=new_value;
        this->siftUp(index);
    }

    void unionize(BinaryHeap *heap_to_union){
        uint32_t size_heap_to_union=heap_to_union->heap.size();
        for(int i=0;i<size_heap_to_union;++i){
            this->insert(heap_to_union->heap[i]);
        }
    }
};

template<typename N>
class BinomialHeap : public Heap<N> {
private:
    struct BinomialHeapNode{
        uint32_t key;
        N value;
        BinomialHeapNode *parent;
        uint32_t degree;
        std::vector<BinomialHeapNode*>*children;
        std::vector<BinomialHeapNode*>*ptr_to_list;

        BinomialHeapNode(const uint32_t key, const N value){
            this->key=key;
            this->value=value;
            this->degree=0;
            this->parent=nullptr;
            this->children=new std::vector<BinomialHeapNode*>();
            this->ptr_to_list=nullptr;
        }
    };

    uint32_t size;
    std::vector<BinomialHeapNode*>list_of_heads;

    BinomialHeapNode* mergeNodes(BinomialHeapNode *ptr1, BinomialHeapNode *ptr2){
        BinomialHeapNode *result;
        if(ptr1->value<ptr2->value){
            ptr1->children->push_back(ptr2);
            ptr2->ptr_to_list=ptr1->children;
            result=ptr1;
            ++ptr1->degree;
        }
        else{
            ptr2->children->push_back(ptr1);
            ptr1->ptr_to_list=ptr2->children;
            result=ptr2;
            ++ptr2->degree;
        }
        return result;
    }

    void mergeLists(std::vector<BinomialHeapNode*>&list_to_merge){
        if(list_to_merge.size()==0){
            return;
        }
        uint32_t i=0;
        BinomialHeapNode *ptrFlag=nullptr;
        std::vector<BinomialHeapNode*>mergedList(BINOMIAL_HEAP_LIST_SIZE, nullptr);
        while(i<BINOMIAL_HEAP_LIST_SIZE){
            if(ptrFlag && !this->list_of_heads[i] && !list_to_merge[i]){
                mergedList[i]=ptrFlag;
                ptrFlag=nullptr;
            }
            else if(!ptrFlag && this->list_of_heads[i] && !list_to_merge[i]){
                mergedList[i]=this->list_of_heads[i];
            }
            else if(!ptrFlag && !this->list_of_heads[i] && list_to_merge[i]){
                mergedList[i]=list_to_merge[i];
            }
            else if(ptrFlag && this->list_of_heads[i] && !list_to_merge[i]){
                ptrFlag=this->mergeNodes(ptrFlag, this->list_of_heads[i]);
            }
            else if(ptrFlag && !this->list_of_heads[i] && list_to_merge[i]){
                ptrFlag=this->mergeNodes(ptrFlag, list_to_merge[i]);
            }
            else if(!ptrFlag && this->list_of_heads[i] && list_to_merge[i]){
                ptrFlag=this->mergeNodes(this->list_of_heads[i], list_to_merge[i]);
            }
            else if(ptrFlag && this->list_of_heads[i] && list_to_merge[i]){
                mergedList[i]=ptrFlag;
                ptrFlag=this->mergeNodes(this->list_of_heads[i], list_to_merge[i]);
            }
            ++i;
        }
        this->list_of_heads=mergedList;
    }

    BinomialHeapNode *findMin() const{
        BinomialHeapNode *result=this->list_of_heads[0];
        for(uint32_t i=1;i<BINOMIAL_HEAP_LIST_SIZE;++i){
            if(this->list_of_heads[i] && (!result || this->list_of_heads[i]->value<result->value)){
                result=this->list_of_heads[i];
            }
        }
        return result;
    }

    void dfsDelete(BinomialHeapNode *ptr){
        if(!ptr){
            return;
        }
        for(uint32_t i=0;i<ptr->children->size();++i){
            this->dfsDelete(ptr->children->at(i));
        }
    }

public:
    BinomialHeap(){
        this->size=0;
        this->list_of_heads=std::vector<BinomialHeapNode*>(BINOMIAL_HEAP_LIST_SIZE, nullptr);
    }

    ~BinomialHeap(){
        for(uint32_t i=0;i<BINOMIAL_HEAP_LIST_SIZE;++i){
            this->dfsDelete(this->list_of_heads[i]);
        }
        this->list_of_heads.clear();
    }

    bool isEmpty() const override {
        if(this->getSize()==0){
            return true;
        }
        return false;
    }

    uint32_t getSize() const override {
        return this->size;
    }

    std::pair<uint32_t, N> getMin() const override {
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to get the smallest element from an empty binomial heap.");
        }
        BinomialHeapNode *result=this->findMin();
        return std::pair<uint32_t, N>(result->key, result->value);
    }

    uint32_t extractMin() override {
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to remove the smallest element from an empty binomial heap.");
        }
        BinomialHeapNode *min=this->findMin();
        const uint32_t key_to_return=min->key;
        std::vector<BinomialHeapNode*>list_of_c(BINOMIAL_HEAP_LIST_SIZE, nullptr);
        for(uint32_t i=0;i<min->children->size();++i){
            list_of_c[min->children->at(i)->degree]=min->children->at(i);
        }
        this->list_of_heads[min->degree]=nullptr;
        min->children->clear();
        delete min->children;
        delete min;
        this->mergeLists(list_of_c);
        --this->size;
        return key_to_return;
    }

    HeapNode insert(const uint32_t key, const N value) override{
        BinomialHeapNode *ptr=new BinomialHeapNode(key, value);
        const BinomialHeapNode *r_ptr=ptr;
        ptr->ptr_to_list=&(this->list_of_heads);
        while(this->list_of_heads[ptr->degree]){
            ptr=this->mergeNodes(ptr, this->list_of_heads[ptr->degree]);
            this->list_of_heads[ptr->degree-1]=nullptr;
        }
        this->list_of_heads[ptr->degree]=ptr;
        ++this->size;
        return HeapNode((void*)r_ptr, key);
    }

    void decreaseKey(HeapNode *heap_node, const N new_value) override{
        if(!heap_node->getPtr()){
            printErrorMsg(2, "DecreaseKey method in a class that represents binary heap received a pointer to NULL.");
        }
        BinomialHeapNode *ptr=(BinomialHeapNode*)heap_node->getPtr();
        if(ptr->value<new_value){
            printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
        }
        ptr->value=new_value;
        BinomialHeapNode *ptr_p=ptr->parent;
        while(ptr_p && ptr->value<ptr_p->value){
            std::swap(ptr->ptr_to_list->at(ptr->degree), ptr_p->ptr_to_list->at(ptr_p->degree));
            std::swap(ptr->ptr_to_list, ptr_p->ptr_to_list);
            ptr=ptr_p;
            ptr_p=ptr->parent;
        }
    }

    void unionize(BinomialHeap *bh) {
        if(bh->isEmpty()){
            delete bh;
            return;
        }
        this->mergeLists(bh->list_of_heads);
        this->size+=bh->size;
        bh->list_of_heads.clear();
        delete bh;
    }
};


template<typename N>
class FibonacciHeap : public Heap<N> {
private:
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

    HeapNode insert(const uint32_t key, const N value) override{
        FibonacciHeapNode *ptr=new FibonacciHeapNode(key,value);
        addToList(ptr,this->min);
        if(this->min==nullptr || this->min->value>value) this->min=ptr;
        ++this->size;
        return HeapNode((void*)ptr, key);
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
        return return_key;
    }

    void decreaseKey(HeapNode *heap_node, const N new_value) override{
        if(!heap_node->getPtr()){
            printErrorMsg(2, "DecreaseKey method in a class that represents binary heap received a pointer to NULL.");
        }
        FibonacciHeapNode *ptr=(FibonacciHeapNode*)heap_node->getPtr();
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
