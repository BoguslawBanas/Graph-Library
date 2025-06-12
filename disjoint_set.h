#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H
#include <vector>
#include <cstdint>
#include "error_info.h"

class Disjoint_Set{
private:
    std::vector<uint32_t>parent;
    std::vector<uint32_t>rank;

public:
    Disjoint_Set(uint32_t size){
        parent=std::vector<uint32_t>(size);
        rank=std::vector<uint32_t>(size, 0);
        for(int32_t i=0;i<size;++i){
            parent[i]=i;
        }
    }

    ~Disjoint_Set()=default;

    uint32_t find(const uint32_t vertex){
        if(vertex>=parent.size()){
            printErrorMsg(2, "Trying to reach an index outside of a size of a Disjoint_set.");
        }

        if(parent[vertex]!=vertex){
            parent[vertex]=find(parent[vertex]);
        }
        return parent[vertex];
    }

    void unionize(const uint32_t v1, const uint32_t v2) {
        if(v1>=parent.size() || v2>=parent.size()){
            printErrorMsg(2, "Trying to reach an index outside of a size of a Disjoint_set.");
        }

        uint32_t fv1=find(v1);
        uint32_t fv2=find(v2);
        if(fv1==fv2){
            return;
        }

        if(rank[fv1]<rank[fv2]){
            parent[fv1]=fv2;
        }
        else if(rank[fv1]>rank[fv2]){
            parent[fv2]=fv1;
        }
        else{
            parent[fv2]=fv1;
            ++rank[fv1];
        }
    }

    inline bool areNodesConnected(const uint32_t v1, const uint32_t v2){
        return find(v1)==find(v2);
    }
};

#endif
