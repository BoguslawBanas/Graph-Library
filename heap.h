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
    virtual bool isEmpty()=0;
    virtual uint32_t getSize()=0;
    virtual void insert(const uint32_t node, const N value)=0;
    virtual std::pair<uint32_t, N> getMin()=0;
    virtual void extractMin()=0;
    virtual bool doesContain(const uint32_t node)=0;
    virtual void decreaseKey(const uint32_t node, const N new_value)=0;
};

template<typename N>
class BinaryHeap : public Heap<N> {
private:
    std::vector<std::pair<uint32_t, N>>heap;
    std::unordered_map<uint32_t, uint32_t>m;

    void swapNodes(const uint32_t node1, const uint32_t node2){
        std::swap(m[heap[node1].first], m[heap[node2].first]);
        std::swap(heap[node1], heap[node2]);
    }

    void minHeapify(const int32_t i){
        const uint32_t size=this->getSize();
        const uint32_t lc=2*i+1;
        const uint32_t rc=2*i+2;
        uint32_t min=i;
        if(lc<size && this->heap[i].second>this->heap[lc].second){
            min=lc;
        }
        if(rc<size && this->heap[min].second>this->heap[rc].second){
            min=rc;
        }

        if(min!=i){
            this->swapNodes(i, min);
            this->minHeapify(min);
        }
    }

public:
    BinaryHeap() {
        heap.clear();
    }

    ~BinaryHeap()=default;

    uint32_t getSize() override{
        return heap.size();
    }

    bool isEmpty() override{
        return heap.empty();
    }

    void insert(const uint32_t node, const N value) override {
        if(this->doesContain(node)){
            printErrorMsg(2, "Trying to insert an element with the same key that exists already in the binary heap.");
        }
        uint32_t index=heap.size();
        heap.push_back(std::pair<uint32_t, N>(node, value));
        m[node]=index;
        while(index!=0 && heap[(index-1)/2].second>heap[index].second) {
            std::swap(m[heap[(index-1)/2].first], m[heap[index].first]);
            std::swap(heap[(index-1)/2], heap[index]);
            index=(index-1)/2;
        }
    }

    std::pair<uint32_t, N> getMin() override {
        if(this->isEmpty()){
            printErrorMsg(2, "Unable to get the smallest element from an empty binary heap.");
        }
        return heap[0];
    }

    void extractMin() override {
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to remove the smallest element from an empty binary heap.");
        }
        m.erase(heap[0].first);
        if(this->getSize()>1){
            std::swap(heap[0], heap[heap.size()-1]);
            m[heap[0].first]=0;
        }
        heap.pop_back();
        uint32_t index=0;
        while(heap.size()>=2*(index+1)) {
            if(heap.size()==2*(index+1) && heap[2*index+1].second<=heap[index].second) {
                this->swapNodes(index, 2*index+1);
                break;
            }
            else if(heap.size()>2*(index+1) && (heap[2*index+1].second<=heap[index].second || heap[2*(index+1)].second<=heap[index].second)) {
                if(heap[2*index+1].second<=heap[2*(index+1)].second) {
                    this->swapNodes(index, 2*index+1);
                    index=2*index+1;
                }
                else {
                    this->swapNodes(index, 2*(index+1));
                    index=2*(index+1);
                }
            }
            else break;
        }
    }

    bool doesContain(const uint32_t node) override {
        if(m.find(node)!=m.end()) return true;
        return false;
    }

    void decreaseKey(const uint32_t node, const N new_value) override {
        if(!(this->doesContain(node))){
            printErrorMsg(1, "The node is not in the binary heap, so method 'insert(const uint32_t, const N)' will take place");
            this->insert(node,new_value);
            return;
        }
        uint32_t index=m[node];
        if(heap[index].second<new_value){
            printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
        }
        heap[index].second=new_value;
        while(index!=0 && heap[(index-1)/2].second>heap[index].second) {
            this->swapNodes(index, (index-1)/2);
            index=(index-1)/2;
        }
    }

    void unionize(BinaryHeap<N> *to_union){
        for(auto &i : to_union->heap){
            if(this->doesContain(i.first)){
                printErrorMsg(2, "In function unionize (class BinaryHeap) there is already a value with that key.");
            }
            m[i.first]=this->getSize();
            this->heap.push_back(i);
        }
        const uint32_t size=this->getSize();
        for(int32_t i=size/2-1;i>=0;--i){
            this->minHeapify(i);
        }
        delete to_union;
    }
};

template<typename N>
class BinomialHeap : public Heap<N> {
private:
    struct BinomialHeapNode{
        uint32_t key;
        N key_value;
        uint32_t degree;
        BinomialHeapNode *child;
        BinomialHeapNode *sibling;
        BinomialHeapNode *parent;

        BinomialHeapNode(const uint32_t key, const N value){
            this->key=key;
            this->key_value=value;
            this->degree=0;
            this->child=this->sibling=this->parent=nullptr;
        }
    };

    BinomialHeapNode *head;
    uint32_t size;
    std::unordered_map<uint32_t, BinomialHeapNode*>m;

    BinomialHeapNode* find_min(){
        if(!this->head) return nullptr;
        BinomialHeapNode *result=this->head;
        BinomialHeapNode *ptr=this->head->sibling;

        while(ptr){
            if(ptr->key_value<result->key_value) result=ptr;
            ptr=ptr->sibling;
        }
        return result;
    }

    void priv_union(){
        BinomialHeapNode *ptr=this->head;
        BinomialHeapNode *prev=nullptr;
        BinomialHeapNode *nxt=this->head->sibling;
        while(nxt!=nullptr){
            if(ptr->degree!=nxt->degree){
                prev=ptr;
                ptr=ptr->sibling;
                nxt=ptr->sibling;
            }
            else if(ptr->sibling->sibling!=nullptr && ptr->degree==nxt->degree && nxt->degree==nxt->sibling->degree){
                prev=ptr;
                ptr=ptr->sibling;
                nxt=ptr->sibling;
            }
            else if(ptr->degree==nxt->degree && ptr->key_value<=nxt->key_value){
                ptr->sibling=nxt->sibling;
                nxt->sibling=ptr->child;
                ptr->child=nxt;
                nxt->parent=ptr;
                nxt=ptr->sibling;
                ++ptr->degree;
            }
            else{
                if(prev==nullptr) this->head=nxt;
                else prev->sibling=nxt;
                ptr->sibling=nxt->child;
                nxt->child=ptr;
                ptr->parent=nxt;
                ++nxt->degree;
                ptr=nxt;
                nxt=ptr->sibling;
            }
        }
    }

    void priv_dfs(BinomialHeapNode *ptr){
        if(ptr==nullptr) return;
        BinomialHeapNode *ptr2=nullptr;
        do{
            ptr2=ptr->sibling;
            priv_dfs(ptr->child);
            delete ptr;
            ptr=ptr2;
        }while(ptr!=nullptr);
    }

public:
    BinomialHeap() {
        this->size=0;
        head=nullptr;
    }

    ~BinomialHeap(){
        priv_dfs(this->head);
    }

    bool isEmpty() override {
        return this->size==0;
    }

    uint32_t getSize() override {
        return this->size;
    }

    void insert(const uint32_t node, const N value) override{
        if(this->doesContain(node)){
            printErrorMsg(2, "Trying to insert an element with the same key that exists already in the binomial heap.");
        }
        BinomialHeapNode *ptr=new BinomialHeapNode(node,value);
        m[node]=ptr;
        if(this->isEmpty()){
            this->head=ptr;
        }
        else{
            ptr->sibling=this->head;
            this->head=ptr;
            priv_union();
        }
        ++this->size;
    }

    std::pair<uint32_t, N> getMin() override{
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to get the smallest element from an empty binomial heap.");
        }
        BinomialHeapNode *min=find_min();
        return std::pair<uint32_t, N>(min->key, min->key_value);
    }

    void extractMin() override{
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to extract the smallest element from an empty binomial heap.");
        }
        BinomialHeapNode *min=find_min();
        m.erase(min->key);
        if(this->getSize()==1){
            m.clear();
            delete head;
            head=nullptr;
            --this->size;
            return;
        }
        BinomialHeapNode *ptr=this->head;
        BinomialHeapNode *children_ptr=min->child;

        std::vector<BinomialHeapNode*>tmp_children;
        std::vector<BinomialHeapNode*>tmp_heads;
        std::vector<BinomialHeapNode*>new_list;

        while(children_ptr!=nullptr){
            tmp_children.push_back(children_ptr);
            children_ptr=children_ptr->sibling;
        }

        while(ptr!=nullptr){
            if(ptr!=min){
                tmp_heads.push_back(ptr);
            }
            ptr=ptr->sibling;
        }

        delete min;

        int32_t i=tmp_children.size()-1;
        int32_t j=0;

        while(i>=0 && j<tmp_heads.size()){
            if(tmp_heads[j]->degree<tmp_children[i]->degree){
                new_list.push_back(tmp_heads[j]);
                ++j;
            }
            else{
                new_list.push_back(tmp_children[i]);
                --i;
            }
        }
        while(i>=0){
            new_list.push_back(tmp_children[i]);
            --i;
        }
        while(j<tmp_heads.size()){
            new_list.push_back(tmp_heads[j]);
            ++j;
        }

        this->head=min=new_list[0];
        new_list.back()->sibling=nullptr;
        for(uint32_t k=1;k<new_list.size();++k){
            new_list[k-1]->sibling=new_list[k];
            if(min->key_value>new_list[k]->key_value){
                min=new_list[k];
            }
        }
        priv_union();
        ptr=this->head;
        while(ptr){
            ptr->parent=nullptr;
            ptr=ptr->sibling;
        }
        --this->size;
    }

    bool doesContain(const uint32_t node) override{
        if(m.find(node)!=m.end()) return true;
        return false;
    }

    void decreaseKey(const uint32_t node, const N new_value) override {
        if(!(this->doesContain(node))){
            printErrorMsg(1, "The node is not in the binomial heap, so method 'insert(const uint32_t, const N)' will take place");
            this->insert(node, new_value);
            return;
        }
        BinomialHeapNode *ptr=m[node];
        if(new_value>ptr->key_value){
            printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
        }
        ptr->key_value=new_value;
        while(ptr->parent!=nullptr && ptr->key_value<ptr->parent->key_value){
            std::swap(m[ptr->key], m[ptr->parent->key]);
            std::swap(ptr->key, ptr->parent->key);
            std::swap(ptr->key_value, ptr->parent->key_value);
            ptr=ptr->parent;
        }
    }

    void unionize(BinomialHeap *to_union) {
        if(to_union->isEmpty()){
            delete to_union;
            return;
        }
        BinomialHeapNode *tmp=head;
        BinomialHeapNode *tmp2=to_union->head;
        BinomialHeapNode *tmp3=nullptr;
        BinomialHeapNode *new_head=nullptr;
        BinomialHeapNode *min=find_min();

        for(auto &it : to_union->m){
            if(this->doesContain(it.first)){
                printErrorMsg(2, "In function unionize (class BinomialHeap) there is already a value with that key.");
            }
            m[it.first]=it.second;
        }

        if(tmp->degree<=tmp2->degree){
            tmp3=tmp;
            tmp=tmp->sibling;
        }
        else{
            tmp3=tmp2;
            tmp2=tmp2->sibling;
        }
        new_head=tmp3;
        while(tmp!=nullptr && tmp2!=nullptr){
            if(tmp->degree<=tmp2->degree){
                tmp3->sibling=tmp;
                tmp=tmp->sibling;
            }
            else{
                tmp3->sibling=tmp2;
                tmp2=tmp2->sibling;
            }
            tmp3=tmp3->sibling;
        }
        while(tmp!=nullptr){
            tmp3->sibling=tmp;
            tmp=tmp->sibling;
            tmp3=tmp3->sibling;
        }
        while(tmp2!=nullptr){
            tmp3->sibling=tmp2;
            tmp2=tmp2->sibling;
            tmp3=tmp3->sibling;
        }
        if(to_union->getMin().second<this->getMin().second){
            min=to_union->m[to_union->getMin().first];
        }
        this->size+=to_union->getSize();
        this->head=new_head;
        this->priv_union();
        to_union->head=nullptr;
        delete to_union;
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
    std::unordered_map<uint32_t, FibonacciHeapNode*>m;
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

    void consolidate(FibonacciHeapNode *ptr, std::vector<int32_t>&consolidate_arr){
        const uint32_t d=ptr->degree;
        if(consolidate_arr[d]==-1){
            consolidate_arr[d]=ptr->key;
        }
        else{
            FibonacciHeapNode *ptr2=m[consolidate_arr[d]];
            consolidate_arr[d]=-1;
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
        m.clear();
        size=0;
    }

    ~FibonacciHeap(){
        priv_dfs(this->min);
    }

    bool isEmpty() override {
        if(this->getSize()==0) return true;
        return false;
    }

    uint32_t getSize() override {
        return this->size;
    }

    void insert(const uint32_t node, const N value) override {
        if(this->doesContain(node)){
            printErrorMsg(2, "Trying to insert an element with the same key that exists already in the fibonacci heap.");
        }
        FibonacciHeapNode *ptr=new FibonacciHeapNode(node,value);
        m[node]=ptr;
        addToList(ptr,this->min);
        if(this->min==nullptr || this->min->value>value) this->min=ptr;
        ++this->size;
    }

    std::pair<uint32_t,N> getMin() override {
        if(this->isEmpty()){
            printErrorMsg(2, "Trying to get the smallest element from an empty fibonacci heap.");
        }
        return std::pair<uint32_t,N> (this->min->key, this->min->value);
    }

    void extractMin() override{
        if(this->isEmpty()) printErrorMsg(2, "Trying to extract the smallest element from an empty fibonacci heap.");
        m.erase(min->key);
        if(this->size==1){
            delete min;
            min=nullptr;
            --this->size;
            return;
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

        std::vector<int32_t>consolidate_arr(CONSOLIDATE_SIZE, -1);
        std::vector<uint32_t>tmp_arr;
        FibonacciHeapNode *it=this->min;
        do{
            tmp_arr.push_back(it->key);
            it=it->right_s;
        }while(it!=this->min);
        
        for(uint32_t i : tmp_arr){
            consolidate(m[i], consolidate_arr);
        }

        this->min=nullptr;

        for(uint32_t i=0;i<CONSOLIDATE_SIZE;++i){
            if(consolidate_arr[i]!=-1){
                FibonacciHeapNode *ptr=m[consolidate_arr[i]];
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

    bool doesContain(const uint32_t node) override{
        if(m.find(node)==m.end()) return false;
        return true;
    }

    void decreaseKey(const uint32_t node, const N value) override {
        if(!(this->doesContain(node))){
            printErrorMsg(1, "The node is not in the fibonacci heap, so method 'insert(const uint32_t, const N)' will take place");
            this->insert(node, value);
            return;
        }
        FibonacciHeapNode *ptr=m[node];
        if(value>ptr->value){
            printErrorMsg(2, "New value is bigger than original one, so operation 'decreaseKey' cannot take place.");
        }
        ptr->value=value;
        if(ptr->parent!=nullptr && value<ptr->parent->value){
            FibonacciHeapNode *p=ptr->parent;
            cutNode(ptr);
            cascadingCutNode(p);
        }
        if(this->min->value>value) this->min=ptr;
    }

    void unionize(FibonacciHeap *to_union) {
        if(to_union->isEmpty()){
            delete to_union;
            return;
        }
        for(auto &it : to_union->m){
            if(this->doesContain(it.first)){
                printErrorMsg(2, "In function unionize (class FibonacciHeap) there is already a value with that key.");
            }
            m[it.first]=it.second;
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
