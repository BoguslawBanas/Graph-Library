#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H
#include <vector>
#include <cstdint>
#include "error_info.h"

class Disjoint_Set{
private:
    std::vector<int32_t>s;
public:
    Disjoint_Set(uint32_t value) {
        s=std::vector<int32_t>(value, -1);
    }

    ~Disjoint_Set()=default;

    uint32_t find(const uint32_t vertex) {
        if(vertex>=s.size()){
            printErrorMsg(2, "Trying to reach an index outside of a size of a Disjoint_set.");
        }
        int32_t tmp=vertex;
        int32_t tmp2=s[vertex];
        while(tmp2!=-1) {
            tmp=tmp2;
            tmp2=s[tmp];
        }
        return tmp;
    }

    void unionize(const uint32_t v1, const uint32_t v2) {
        if(v1>=s.size() || v2>=s.size()){
            printErrorMsg(2, "Trying to reach an index outside of a size of a Disjoint_set.");
        }
        uint32_t fv1=find(v1);
        uint32_t fv2=find(v2);
        if(fv1<fv2) s[fv2]=fv1;
        else s[fv1]=fv2;
    }

    void unionize_f(const uint32_t fv1, const uint32_t fv2){
        if(fv1<fv2) s[fv2]=fv1;
        else s[fv1]=fv2;
    }
};

#endif
