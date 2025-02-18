#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <queue>
#include <cstdint>
#include <unordered_map>
#include <algorithm>

template<typename G>
bool isWeaklyConnected(G &g){
    if(g.getSize()==0) return false;
    if(!g.isDirected()){
        return isStronglyConnected(g);
    }
    uint32_t counter=1;
    std::queue<uint32_t>q;
    std::vector<std::vector<uint32_t>>v1(g.getSize(), std::vector<uint32_t>());
    for(uint32_t i=0;i<g.getSize();++i){
        for(uint32_t j : g.getNeighbours(i)){
            v1[j].push_back(i);
        }
    }
    std::vector<bool>is_visited(g.getSize(), false);
    is_visited[0]=true;
    q.push(0);
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(!is_visited[i]){
                is_visited[i]=true;
                q.push(i);
                ++counter;
            }
        }
        for(uint32_t i : v1[q.front()]){
            if(!is_visited[i]){
                is_visited[i]=true;
                q.push(i);
                ++counter;
            }
        }
        q.pop();
    }
    if(counter==g.getSize()) return true;
    return false;
}

template<typename G>
bool isStronglyConnected(G &g){
    if(g.getSize()==0) return false;
    uint32_t counter=1;
    std::queue<uint32_t>q;
    std::vector<bool>v(g.getSize(), false);
    v[0]=true;
    q.push(0);
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(!v[i]){
                v[i]=true;
                q.push(i);
                ++counter;
            }
        }
        q.pop();
    }
    if(counter!=g.getSize()) return false;
    if(!g.isDirected()) return true;

    //for directed Gs
    g.transpose();
    counter=1;
    for(uint32_t i=0;i<g.getSize();++i) v[i]=false;
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(!v[i]){
                v[i]=true;
                q.push(i);
                ++counter;
            }
        }
        q.pop();
    }
    g.transpose();
    if(counter==g.getSize()) return true;
    return false;
}

template<typename G>
bool isBipartite(G &g){
    if(g.getSize()==0) return false;
    for(uint32_t i=0;i<g.getSize();++i) {
        if(g.areVertexesConnected(i,i)) return false;
    }
    std::vector<uint8_t>tmp(g.getSize(), 0);
    std::queue<uint32_t>q;

    for(uint32_t k=0;k<g.getSize();++k){
        if(tmp[k]==0){
            q.push(k);
            tmp[k]=1;
            while(!q.empty()){
                for(uint32_t i : g.getNeighbours(q.front())){
                    if(tmp[i]==0){
                        tmp[q.front()]==1 ? tmp[i]=2 : tmp[i]=1;
                        q.push(i);
                    }
                    else if(tmp[q.front()]==tmp[i]) return false;
                }
                q.pop();
            }
        }
    }
    return true;
}

template<typename G>
int32_t bfs(G &g, const uint32_t src, const uint32_t destination){
    if(src==destination) return 0;
    std::queue<uint32_t>q;
    std::vector<int32_t>tmp(g.getSize(), -1);
    tmp[src]=0;
    q.push(src);
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(tmp[i]==-1){
                if(i==destination) return tmp[q.front()]+1;
                tmp[i]=tmp[q.front()]+1;
                q.push(i);
            }
        }
        q.pop();
    }
    return -1;
}

template<typename G>
std::unordered_map<uint32_t, int32_t>* bfs(G& g, const uint32_t src, const std::vector<uint32_t>& destinations){
    if(destinations.empty()){
        return new std::unordered_map<uint32_t, int32_t>();
    }
    auto *result=new std::unordered_map<uint32_t, int32_t>(destinations.size());
    std::vector<bool>is_important(g.getSize(), false);
    std::vector<int32_t>tmp(g.getSize(), -1);
    for(uint32_t i=0;i<destinations.size();++i){
        result->insert({destinations[i],-1});
        is_important[destinations[i]]=true;
    }
    std::queue<uint32_t>q;
    uint32_t counter=0;
    tmp[src]=0;
    if(is_important[src]){
        ++counter;
        result->at(src)=0;
    }
    q.push(src);
    while(!q.empty() && counter!=destinations.size()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(tmp[i]==-1){
                tmp[i]=tmp[q.front()]+1;
                q.push(i);
                if(is_important[i]){
                    ++counter;
                    result->at(i)=tmp[i];
                }
            }
        }
        q.pop();
    }
    return result;
}

template<typename G>
std::vector<int32_t>* bfs(G &g, const uint32_t src){
    auto result=new std::vector<int32_t>(g.getSize(), -1);
    std::queue<uint32_t>q;
    q.push(src);
    result->at(src)=0;
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(result->at(i)==-1){
                result->at(i)=result->at(q.front())+1;
                q.push(i);
            }
        }
        q.pop();
    }
    return result;
}

template<typename G, typename F>
int32_t bfs(G &g, const uint32_t src, const uint32_t destination, const F &fun){
    if(src==destination) return 0;
    std::queue<int32_t>q;
    std::vector<int32_t>tmp(g.getSize(), -1);
    tmp[src]=0;
    q.push(src);
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(tmp[i]==-1 && fun(i, tmp[q.front()]+1)){
                if(i==destination) return tmp[q.front()]+1;
                tmp[i]=tmp[q.front()]+1;
                q.push(i);
            }
        }
        q.pop();
    }
    return -1;
}

template<typename G, typename F>
std::unordered_map<uint32_t, int32_t>* bfs(G& g, const uint32_t src, const std::vector<uint32_t>& destinations, const F &fun){
    if(destinations.empty()){
        return new std::unordered_map<uint32_t, int32_t>();
    }
    auto *result=new std::unordered_map<uint32_t, int32_t>(destinations.size());
    std::vector<bool>is_important(g.getSize(), false);
    std::vector<int32_t>tmp(g.getSize(), -1);
    for(uint32_t i=0;i<destinations.size();++i){
        result->insert({destinations[i],-1});
        is_important[destinations[i]]=true;
    }
    std::queue<uint32_t>q;
    uint32_t counter=0;
    tmp[src]=0;
    if(is_important[src]){
        ++counter;
        result->at(src)=0;
    }
    q.push(src);
    while(!q.empty() && counter!=destinations.size()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(tmp[i]==-1 && fun(i, tmp[q.front()]+1)){
                tmp[i]=tmp[q.front()]+1;
                q.push(i);
                if(is_important[i]){
                    ++counter;
                    result->at(i)=tmp[i];
                }
            }
        }
        q.pop();
    }
    return result;
}

template<typename G, typename F>
std::vector<int32_t>* bfs(G &g, const uint32_t src, const F &fun){
    auto result=new std::vector<int32_t>(g.getSize(), -1);
    std::queue<uint32_t>q;
    q.push(src);
    result->at(src)=0;
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(result->at(i)==-1 && fun(i, result->at(q.front()+1))){
                result->at(i)=result->at(q.front())+1;
                q.push(i);
                std::cout<<i<<'\n';
            }
        }
        q.pop();
    }
    return result;
}

template<typename G, typename F>
void bfs(G &g, const uint32_t src, F &f){
    std::vector<bool>is_visited(false, g.getSize());
    std::queue<uint32_t>q;
    q.push(src);
    is_visited[src]=true;
    while(!q.empty()){
        f(q.front());
        for(uint32_t i : g.getNeighbours(q.front())){
            if(!is_visited[i]){
                is_visited[i]=true;
                q.push(i);
            }
        }
        q.pop();
    }
}

template<typename G>
std::vector<int32_t>* bfs_path(G &g, const uint32_t src, const uint32_t destination){
    std::vector<int32_t>*result=new std::vector<int32_t>();
    bool flag=false;
    if(src==destination){
        result->push_back(src);
        return result;
    }
    std::queue<uint32_t>q;
    std::vector<int32_t>tmp(g.getSize(), -2);
    tmp[src]=-1;
    q.push(src);
    while(!q.empty()){
        for(uint32_t i : g.getNeighbours(q.front())){
            if(tmp[i]==-2){
                tmp[i]=q.front();
                if(i==destination){
                    flag=true;
                    break;
                }
                q.push(i);
            }
        }
        q.pop();
    }
    if(!flag){
        delete result;
        return nullptr;
    }
    uint32_t i=destination;
    do{
        result->push_back(i);
        i=tmp[i];
    }while(i!=-1);
    std::reverse(result->begin(), result->end());
    return result;
}

template<typename G, typename F>
std::vector<uint32_t>* bfs_path(G &g, const uint32_t src, const uint32_t destination, const F &fun) {
    std::vector<int32_t>tmp(g.getSize(), -2);
    std::vector<uint32_t>tmp_2(g.getSize(),0);
    std::queue<uint32_t>q;
    q.push(src);
    tmp[src]=-1;
    while (!q.empty() && q.front()!=destination) {
        for(uint32_t i : g.getNeighbours(q.front())) {
            if(tmp[i]==-2 && fun(i, tmp_2[q.front()]+1)) {
                tmp[i]=q.front();
                tmp_2[i]=tmp_2[q.front()]+1;
                q.push(i);
            }
        }
        q.pop();
    }
    if(tmp[destination]==-2) {
        return nullptr;
    }
    std::vector<uint32_t>*result=new std::vector<uint32_t>();
    int32_t t=destination;
    do {
        result->push_back(t);
        t=tmp[t];
    }while(t!=-1);
    std::reverse(result->begin(), result->end());
    return result;
}

namespace{
    template<typename G, typename F>
    void dfs_preorder(G &g, const uint32_t src, F &f, std::vector<bool>&is_visited){
        if(is_visited[src]) return;
        is_visited[src]=true;
        f(src);
        for(uint32_t i : g.getNeighbours(src)){
            dfs_preorder(g, i, f, is_visited);
        }
    }

    template<typename G, typename F>
    void dfs_postorder(G &g, const uint32_t src, F &f, std::vector<bool>&is_visited){
        if(is_visited[src]) return;
        is_visited[src]=true;
        for(uint32_t i : g.getNeighbours(src)){
            dfs_postorder(g, i, f, is_visited);
        }
        f(src);
    }
}

template<typename G, typename F>
void dfs_preorder(G &g, const uint32_t src, F &f){
    std::vector<bool>is_visited(g.getSize(), false);
    dfs_preorder(g, src, f, is_visited);
}

template<typename G, typename F>
void dfs_postorder(G &g, const uint32_t src, F &f){
    std::vector<bool>is_visited(g.getSize(), false);
    dfs_postorder<G,F>(g, src, f, is_visited);
}

template<typename G, typename N, typename PQ>
std::vector<std::pair<uint32_t, uint32_t>>* prim(G &g, const uint32_t src) {
    auto result=new std::vector<std::pair<uint32_t,uint32_t>>();
    std::vector<int32_t>parent(g.getSize(), -1);
    std::vector<N>keys(g.getSize(), g.getMax());
    std::vector<bool>mstSet(g.getSize(), false);

    keys[src]=-1;
    PQ pq;
    pq.push(src, 0);
    uint32_t counter=0;
    bool is_not_first=false;
    while(!pq.empty() && counter!=g.getSize()-1){
        uint32_t v=pq.top().first;
        pq.pop();
        mstSet[v]=true;
        for(auto &i : g.getNeighboursWithWeights(v)){
            if(!mstSet[i.first] && i.second<keys[i.first]){
                keys[i.first]=i.second;
                pq.push(i.first, i.second);
                parent[i.first]=v;
            }
        }
        if(is_not_first){
            result->push_back(std::pair<uint32_t, uint32_t>(parent[v], v));
        }
        else is_not_first=true;
    }
    if(result->size()!=g.getSize()-1){
        delete result;
        return nullptr;
    }
    return result;
}

template<typename G, typename N, typename DS>
std::vector<std::pair<uint32_t, uint32_t>>* kruskal(G &g){
    if(!isWeaklyConnected<G>(g)) return NULL;
    auto list=g.getListOfEdges();
    std::sort(list.begin(), list.end(), [](const auto &p1, const auto &p2){
        return p1.second<p2.second;
    });
    DS ds(g.getSize());
    auto result=new std::vector<std::pair<uint32_t,uint32_t>>();
    for(auto &i : list){
        if(ds.find(i.first.first)!=ds.find(i.first.second)){
            ds.unionize(i.first.first, i.first.second);
            result->push_back(i.first);
        }
    }
    return result;
}

template<typename G, typename N, typename PQ>
N dijkstra(G &g, const uint32_t src, const uint32_t destination) {
    N max=g.getMax();
    std::vector<bool>is_visited(g.getSize(), false);
    std::vector<N>distance(g.getSize(), max);
    distance[src]=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && !is_visited[destination]) {
        is_visited[pq.top().first]=true;
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[pq.top().first]+i.second) {
                distance[i.first]=distance[pq.top().first]+i.second;
                pq.push(i.first, distance[i.first]);
            }
        }
        pq.pop();
    }
    return distance[destination];
}

template<typename G, typename N, typename PQ>
N dijkstra(G &g, const uint32_t src, const uint32_t destination, bool (*fun)(const uint32_t, N)) {
    N max=g.getMax();
    std::vector<bool>is_visited(g.getSize(), false);
    std::vector<N>distance(g.getSize(), max);
    distance[src]=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && !is_visited[destination]) {
        is_visited[pq.top().first]=true;
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[pq.top().first]+i.second && fun(i.first, distance[pq.top().first]+i.second)) {
                distance[i.first]=distance[pq.top().first]+i.second;
                pq.push(i.first, distance[i.first]);
            }
        }
        pq.pop();
    }
    return distance[destination];
}

template<typename G, typename N, typename PQ>
std::vector<uint32_t>* dijkstra_path(G &g, const uint32_t src, const uint32_t destination) {
    N max=g.getMax();
    std::vector<bool>is_visited(g.getSize(), false);
    std::vector<N>distance(g.getSize(), max);
    std::vector<int32_t>neighbours(g.getSize(), -1);
    distance[src]=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && !is_visited[destination]) {
        is_visited[pq.top().first]=true;
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[pq.top().first]+i.second) {
                distance[i.first]=distance[pq.top().first]+i.second;
                neighbours[i.first]=pq.top().first;
                pq.push(i.first, distance[i.first]);
            }
        }
        pq.pop();
    }
    auto *result=new std::vector<uint32_t>();
    int32_t tmp=destination;
    do {
        result->push_back(tmp);
        tmp=neighbours[tmp];
    }while(tmp!=-1);
    std::reverse(result->begin(), result->end());
    return result;
}

template<typename G, typename N, typename PQ, typename F>
std::vector<uint32_t>* dijkstra_path(G &g, const uint32_t src, const uint32_t destination, bool (*fun)(uint32_t, N)) {
    N max=g.getMax();
    std::vector<bool>is_visited(g.getSize(), false);
    std::vector<N>distance(g.getSize(), max);
    std::vector<int32_t>neighbours(g.getSize(), -1);
    distance[src]=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && !is_visited[destination]) {
        is_visited[pq.top().first]=true;
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[pq.top().first]+i.second && fun(i.first, distance[pq.top().first]+i.second)) {
                distance[i.first]=distance[pq.top().first]+i.second;
                neighbours[i.first]=pq.top().first;
                pq.push(i.first, distance[i.first]);
            }
        }
        pq.pop();
    }
    auto *result=new std::vector<uint32_t>();
    int32_t tmp=destination;
    do {
        result->push_back(tmp);
        tmp=neighbours[tmp];
    }while(tmp!=-1);
    uint32_t i=0;
    uint32_t j=result->size()-1;
    while(i<j) {
        std::swap(result->at(i), result->at(j));
        ++i;
        --j;
    }
    return result;
}


template<typename G, typename N, typename PQ>
std::unordered_map<uint32_t, N>* dijkstra(G &g, const uint32_t src, const std::vector<uint32_t> &destinations) {
    N max=g.getMax();
    std::vector<N>distance(g.getSize(), max);
    std::vector<bool>is_visited(g.getSize(), false);
    auto *result=new std::unordered_map<uint32_t, N>();
    std::vector<bool>is_important(g.getSize(), false);
    for(auto &i : destinations) {
        is_important[i]=true;
        result->insert({i, max});
    }
    distance[src]=0;
    uint32_t counter=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && counter!=destinations.size()) {
        is_visited[pq.top().first]=true;
        if(is_important[pq.top().first]) {
            result->at(pq.top().first)=pq.top().second;
            ++counter;
        }
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[pq.top().first]+i.second) {
                distance[i.first]=distance[pq.top().first]+i.second;
                pq.push(i.first, distance[i.first]);
            }
        }
        pq.pop();
    }
    return result;
}

template<typename G, typename N, typename PQ>
std::unordered_map<uint32_t, N>* dijkstra(G &g, const uint32_t src, const std::vector<uint32_t> &destinations, bool (*fun)(uint32_t, N)) {
    N max=g.getMax();
    std::vector<N>distance(g.getSize(), max);
    std::vector<bool>is_visited(g.getSize(), false);
    auto *result=new std::unordered_map<uint32_t, N>();
    std::vector<bool>is_important(g.getSize(), false);
    for(auto &i : destinations) {
        is_important[i]=true;
        result->insert({i, max});
    }
    distance[src]=0;
    uint32_t counter=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && counter!=destinations.size()) {
        is_visited[pq.top().first]=true;
        if(is_important[pq.top().first]) {
            result->at(pq.top().first)=pq.top().second;
            ++counter;
        }
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[pq.top().first]+i.second && fun(i.first, result->at(pq.top().first)+i.second)) {
                distance[i.first]=distance[pq.top().first]+i.second;
                pq.push(i.first, distance[i.first]);
            }
        }
        pq.pop();
    }
    return result;
}

template<typename G, typename N, typename PQ>
std::vector<N>* dijkstra(G &g, const uint32_t src) {
    N max=g.getMax();
    auto *result=new std::vector<N>(g.getSize(), max);
    std::vector<bool>is_visited(g.getSize(), false);
    PQ pq;
    pq.push(src,0);
    result->at(src)=0;
    while(!pq.empty()) {
        is_visited[pq.top().first]=true;
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && result->at(i.first)>result->at(pq.top().first)+i.second) {
                result->at(i.first)=result->at(pq.top().first)+i.second;
                pq.push(i.first, result->at(i.first));
            }
        }
        pq.pop();
    }
    return result;
}

template<typename G, typename N, typename PQ>
std::vector<N>* dijkstra(G &g, const uint32_t src, bool (*fun)(uint32_t, N)) {
    N max=g.getMax();
    auto *result=new std::vector<N>(g.getSize(), max);
    std::vector<bool>is_visited(g.getSize(), false);
    PQ pq;
    pq.push(src,0);
    result->at(src)=0;
    while(!pq.empty()) {
        is_visited[pq.top().first]=true;
        for(auto &i : g.getNeighboursWithWeights(pq.top().first)) {
            if(!is_visited[i.first] && result->at(i.first)>result->at(pq.top().first)+i.second && fun(i.first, result->at(pq.top().first)+i.second)) {
                result->at(i.first)=result->at(pq.top().first)+i.second;
                pq.push(i.first, result->at(i.first));
            }
        }
        pq.pop();
    }
    return result;
}

template<typename G, typename N>
std::vector<N>* bellmanFord(G &g, const uint32_t src) {
    N max=g.getMax();
    auto result=new std::vector<N>(g.getSize(), max);
    result->at(src)=0;
    for(uint32_t i=0;i<g.getSize()-1;++i) {
        bool flag=true;
        for(uint32_t j=0;j<g.getSize();++j) {
            if(result->at(j)!=max) {
                for(auto k : g.getNeighboursWithWeights(j)) {
                    if(result->at(k.first)>k.second+result->at(j)) {
                        result->at(k.first)=k.second+result->at(j);
                        flag=false;
                    }
                }
            }
        }
        if(flag) break;
    }

    for(uint32_t j=0;j<g.getSize();++j) {
        if(result->at(j)!=max) {
            for(auto k : g.getNeighboursWithWeights(j)) {
                if(result->at(k.first)>k.second+result->at(j)) {
                    delete result;
                    return nullptr;
                }
            }
        }
    }
    return result;
}

template<typename G, typename N>
std::vector<std::vector<N>>* floydWarshall(G &g){
    N max=g.getMax();
    std::vector<std::vector<N>>* result=new std::vector<std::vector<N>>(g.getSize(), std::vector<N>(g.getSize(), max));

    for(uint32_t i=0;i<g.getSize();++i) {
        result->at(i)[i]=0;
        for(auto j : g.getNeighboursWithWeights(i)) result->at(i)[j.first]=j.second;
    }

    for(uint32_t i=0;i<g.getSize();++i) {
        for(uint32_t j=0;j<g.getSize();++j) {
            for(uint32_t k=0;k<g.getSize();++k) {
                if(result->at(i)[k]!=max && result->at(j)[i]!=max && result->at(j)[k]>result->at(i)[k]+result->at(j)[i])
                    result->at(j)[k]=result->at(i)[k]+result->at(j)[i];
            }
        }
    }

    for(uint32_t i=0;i<g.getSize();++i) {
        if(result->at(i)[i]<0) {
            delete result;
            return nullptr;
        }
    }
    return result;
}

template<typename G, typename N, typename PQ>
N A_star(G &g, const uint32_t src, const uint32_t destination, N (*heuristic)(const uint32_t)) {
    N max=g.getMax();
    std::vector<bool>is_visited(g.getSize(), false);
    std::vector<N>distance(g.getSize(), max);
    distance[src]=0;
    PQ pq;
    pq.push(src, heuristic(src));
    while(!pq.empty() && !is_visited[destination]) {
        auto front=pq.top();
        pq.pop();
        is_visited[front.first]=true;
        for(auto &i : g.getNeighboursWithWeights(front.first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[front.first]+i.second){
                pq.push(i.first, distance[front.first]+i.second+heuristic(i.first));
                distance[i.first]=distance[front.first]+i.second;
            }
        }
    }
    return distance[destination];
}

template<typename G, typename N, typename PQ>
std::vector<uint32_t>* A_star_path(G &g, const uint32_t src, const uint32_t destination, N (*heuristic)(const uint32_t)) {
    N max=g.getMax();
    std::vector<bool>is_visited(g.getSize(), false);
    std::vector<int32_t>prev_vertex(g.getSize(), -1);
    std::vector<N>distance(g.getSize(), max);
    std::vector<uint32_t>*path=new std::vector<uint32_t>();
    distance[src]=0;
    PQ pq;
    pq.push(src,0);
    while(!pq.empty() && !is_visited[destination]){
        auto front=pq.top();
        pq.pop();
        is_visited[front.first]=true;
        for(auto &i : g.getNeighboursWithWeights(front.first)) {
            if(!is_visited[i.first] && distance[i.first]>distance[front.first]+i.second){
                pq.push(i.first, distance[front.first]+i.second+heuristic(i.first));
                distance[i.first]=distance[front.first]+i.second;
                prev_vertex[i.first]=front.first;
            }
        }
    }
    if(is_visited[destination]==-1){
        delete path;
        return nullptr;
    }
    int32_t tmp=destination;
    do {
        path->push_back(tmp);
        tmp=prev_vertex[tmp];
    }while(tmp!=-1);
    std::reverse(path->begin(), path->end());
    return path;
}

#endif
