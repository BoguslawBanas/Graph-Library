#ifndef GRAPH_H
#define GRAPH_H

#include <cstdint>
#include <vector>
#include <cstdio>
#include <fstream>
#include "error_info.h"

class Graph{
protected:
    bool is_directed;
    bool is_weighted;

    bool isIndexOutOfBounds(const uint32_t idx) const{
        if(idx>=this->getSize()) return true;
        return false;
    }
public:
    bool isDirected() const{
        return this->is_directed;
    }
    bool isWeighted() const{
        return this->is_weighted;
    }
    virtual uint32_t getSize() const=0;
    virtual bool areVertexesConnected(const uint32_t, const uint32_t) const=0;
    void transpose() {
        return;
    }
    virtual uint32_t addVertex()=0;
    virtual void deleteEdge(const uint32_t, const uint32_t)=0;
    virtual const std::vector<uint32_t> getNeighbours(const uint32_t) const=0;
};

class GraphUnweighted : public Graph{
public:
    virtual void addEdge(const uint32_t, const uint32_t)=0;
    virtual const std::vector<std::pair<uint32_t, uint32_t>> getListOfEdges() const=0;
};

template <typename N>
class WeightedGraph : public Graph{
protected:
    N max;
public:
    N getMax() const {
        return this->max;
    }
    virtual const std::vector<std::pair<uint32_t, N>> getNeighboursWithWeights(const uint32_t) const=0;
    virtual void addEdge(const uint32_t, const uint32_t, const N&)=0;
    virtual N getWeight(const uint32_t, const uint32_t) const=0;
    virtual const std::vector<std::pair<std::pair<uint32_t, uint32_t>, N>> getListOfEdges() const=0;
};

class ListGraphDirected : public GraphUnweighted{
private:
    std::vector<std::vector<uint32_t>> adjacencyList;
public:
    ListGraphDirected(const uint32_t size){
        for(uint32_t i=0;i<size;++i) adjacencyList.push_back(std::vector<uint32_t>());
        this->is_directed=true;
        this->is_weighted=false;
    }

    ~ListGraphDirected()=default;

    void transpose(){
        std::vector<std::vector<uint32_t>> tmp(this->getSize(), std::vector<uint32_t>());
        for(uint32_t i=0;i<this->getSize();++i) {
            for(auto j : adjacencyList[i]) {
                tmp[j].push_back(i);
            }
        }
        for(uint32_t i=0;i<this->getSize();++i) {
            adjacencyList[i]=tmp[i];
        }
    }

    uint32_t getSize() const override{
        return adjacencyList.size();
    }

    bool areVertexesConnected(const uint32_t first, const uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class ListGraphDirected is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class ListGraphDirected is out of bounds.");
        }
        for(uint32_t v : adjacencyList[first]){
            if(v==second) return true;
        }
        return false;
    }

    uint32_t addVertex() override{
        adjacencyList.push_back(std::vector<uint32_t>());
        return adjacencyList.size()-1;
    }

    void deleteEdge(const uint32_t first, const uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphDirected is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphDirected is out of bounds.");
        }
        for(int32_t i=0;i<adjacencyList[first].size();++i){
            if(adjacencyList[first][i]==second){
                adjacencyList[first].erase(adjacencyList[first].begin()+i);
                --i;
            }
        }
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class ListGraphDirected is out of bounds.");
        }
        return adjacencyList[vertex];
    }

    void addEdge(const uint32_t first, const uint32_t second) override{
        this->addEdge(first, second, false);
    }

    void addEdge(const uint32_t first, const uint32_t second, const bool saveFlag){
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(const uint32_t, const uint32_t) in a class ListGraphDirected is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(const uint32_t, const uint32_t) in a class ListGraphDirected is out of bounds.");
        }

        bool flag=false;
        if(saveFlag){
            for(auto i : adjacencyList[first]){
                if(second==i){
                    flag=true;
                    break;
                }
            }
        }
        
        if(!flag) adjacencyList[first].push_back(second);
    }

    const std::vector<std::pair<uint32_t, uint32_t>> getListOfEdges() const override {
        std::vector<std::pair<uint32_t, uint32_t>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(auto j : adjacencyList[i]) {
                result.push_back(std::pair<uint32_t, uint32_t>(i,j));
            }
        }
        return result;
    }
};

template <typename N>
class ListGraphWeightedAndDirected : public WeightedGraph<N>{
private:
    std::vector<std::vector<std::pair<uint32_t,N>>> adjacencyList;
public:
    ListGraphWeightedAndDirected(const uint32_t size, const N &max){
        for(uint32_t i=0;i<size;++i){
            adjacencyList.push_back(std::vector<std::pair<uint32_t, N>>());
        }
        this->is_directed=true;
        this->is_weighted=true;
        this->max=max;
    }

    ~ListGraphWeightedAndDirected()=default;

    void transpose(){
        std::vector<std::vector<std::pair<uint32_t, N>>> tmp(this->getSize(), std::vector<std::pair<uint32_t, N>>());
        for(uint32_t i=0;i<this->getSize();++i) {
            for(auto j : adjacencyList[i]) {
                tmp[j.first].push_back(std::pair<uint32_t, N>(i,j.second));
            }
        }
        for(uint32_t i=0;i<this->getSize();++i) {
            adjacencyList[i]=tmp[i];
        }
    }

    uint32_t getSize() const override{
        return adjacencyList.size();
    }

    bool areVertexesConnected(const uint32_t first, const uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        for(auto i : adjacencyList[first]){
            if(i.first==second) return true;
        }
        return false;
    }

    uint32_t addVertex() override{
        adjacencyList.push_back(std::vector<std::pair<uint32_t, N>>());
        return adjacencyList.size()-1;
    }

    void deleteEdge(const uint32_t first, const uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        for(int32_t i=0;i<adjacencyList[first].size();++i){
            if(adjacencyList[first][i].first==second){
                adjacencyList[first].erase(adjacencyList[first].begin()+i);
                --i;
            }
        }
    }

    bool deleteEdge(const uint32_t first, const uint32_t second, const N &weight){
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }

        bool result=false;

        for(int32_t i=0;i<adjacencyList[first].size();++i){
            if(adjacencyList[first][i].first==second && adjacencyList[first][i].second==weight){
                adjacencyList[first].erase(adjacencyList[first].begin()+i);
                result=true;
                break;
            }
        }

        return result;
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        std::vector<uint32_t>result;
        for(auto v : adjacencyList[vertex]) {
            result.push_back(v.first);
        }
        return result;
    }

    const std::vector<std::pair<uint32_t,N>> getNeighboursWithWeights(const uint32_t vertex) const override {
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighboursWithWeights(const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        return adjacencyList[vertex];
    }

    void addEdge(const uint32_t v1, const uint32_t v2, const N &weight) override{
        addEdge(v1,v2,weight,false);
    }

    void addEdge(const uint32_t v1, const uint32_t v2, const N &weight, const bool saveFlag){
        if(this->isIndexOutOfBounds(v1)){
            printErrorMsg(2, "The first argument of a method addEdge(const uint32_t, const uint32_t, const N) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(v2)){
            printErrorMsg(2, "The second argument of a method addEdge(const uint32_t, const uint32_t, const N) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        bool flag=false;
        if(saveFlag){
            for(auto i : adjacencyList[v1]){
                if(i.first==v2){
                    i.second=weight;
                    flag=true;
                    break;
                }
            }
        }
        if(!flag) adjacencyList[v1].push_back(std::pair<uint32_t,N>(v2,weight));
    }

    N getWeight(const uint32_t first, const uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method getWeight(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method getWeight(const uint32_t, const uint32_t) in a class ListGraphWeightedAndDirected<N> is out of bounds.");
        }

        N result=this->max;
        for(auto v : adjacencyList[first]) {
            if(v.first==second && v.second<result){
                result=v.second;
            } 
        }
        return result;
    }

    const std::vector<std::pair<std::pair<uint32_t, uint32_t>, N>> getListOfEdges() const override {
        std::vector<std::pair<std::pair<uint32_t, uint32_t>, N>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(auto j : adjacencyList[i]) {
                result.push_back(std::pair<std::pair<uint32_t,uint32_t>,N>(std::pair<uint32_t, uint32_t>(i,j.first),j.second));
            }
        }
        return result;
    }
};

template <typename N>
class ListGraphWeighted : public WeightedGraph<N>{
private:
    std::vector<std::vector<std::pair<uint32_t,N>>> adjacencyList;
public:
    ListGraphWeighted(const uint32_t size, const N &max){
        for(uint32_t i=0;i<size;++i) adjacencyList.push_back(std::vector<std::pair<uint32_t,N>>());
        this->is_directed=false;
        this->is_weighted=true;
        this->max=max;
    }

    ~ListGraphWeighted()=default;


    uint32_t getSize() const override{
        return adjacencyList.size();
    }

    bool areVertexesConnected(uint32_t first, uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first,second);
        for(auto i : adjacencyList[first]) {
            if(i.first==second) return true;
        }
        return false;
    }

    uint32_t addVertex() override{
        adjacencyList.push_back(std::vector<std::pair<uint32_t, N>>());
        return adjacencyList.size()-1;
    }

    void deleteEdge(uint32_t first, uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first, second);

        bool flag=false;
        for(int32_t i=0;i<adjacencyList[first].size();++i){
            if(adjacencyList[first][i].first==second){
                adjacencyList[first].erase(adjacencyList[first].begin()+i);
                flag=true;
                --i;
            }
        }

        if(flag){
            for(int32_t i=0;i<adjacencyList[second].size();++i) {
                if(adjacencyList[second][i].first==first) {
                    adjacencyList[second].erase(adjacencyList[second].begin()+i);
                    --i;
                }
            }
        }
    }

    bool deleteEdge(uint32_t first, uint32_t second, const N &weight){
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }

        bool result=false;
        if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first, second);

        for(int i=0;i<adjacencyList[first].size();++i){
            if(adjacencyList[first][i].second==weight){
                adjacencyList[first].erase(adjacencyList[first].begin()+i);
                result=true;
                break;
            }
        }
        
        if(result){
            for(int i=0;i<adjacencyList[second].size();++i){
                if(adjacencyList[second][i].second==weight){
                    adjacencyList[second].erase(adjacencyList[second].begin()+i);
                    break;
                }
            }
        }

        return result;
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        std::vector<uint32_t>result;
        for(auto i : adjacencyList[vertex]) {
            result.push_back(i.first);
        }
        return result;
    }

    const std::vector<std::pair<uint32_t,N>> getNeighboursWithWeights(const uint32_t vertex) const override {
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighboursWithWeights(const uint32_t) in a class ListGraphWeighted<N> is out of bounds.");
        }
        return adjacencyList[vertex];
    }

    void addEdge(uint32_t v1, uint32_t v2, const N &weight) override{
        addEdge(v1,v2,weight,false);
    }

    void addEdge(uint32_t v1, uint32_t v2, const N &weight, const bool saveFlag){
        if(this->isIndexOutOfBounds(v1)){
            printErrorMsg(2, "The first argument of a method addEdge(uint32_t, uint32_t, const N) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(v2)){
            printErrorMsg(2, "The second argument of a method addEdge(uint32_t, uint32_t, const N) in a class ListGraphWeighted<N> is out of bounds.");
        }
        bool flag=false;
        if(saveFlag){
            if(adjacencyList[v1].size()>adjacencyList[v2].size()) std::swap(v1,v2);
            for(auto i : adjacencyList[v1]){
                if(i.first==v2){
                    i.second=weight;
                    flag=true;
                    break;
                }
            }
        }
        
        if(flag){
            for(auto i : adjacencyList[v2]){
                if(i.first==v1){
                    i.second=weight;
                    break;
                }
            }
        }
        else{
            adjacencyList[v1].push_back(std::pair<uint32_t, N>(v2, weight));
            if(v1!=v2){
                adjacencyList[v2].push_back(std::pair<uint32_t, N>(v1, weight));
            }
        }
    }

    N getWeight(uint32_t first, uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(uint32_t, uint32_t, const N) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(uint32_t, uint32_t, const N) in a class ListGraphWeighted<N> is out of bounds.");
        }
        if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first, second);
        N weight=this->max;

        for(auto i : adjacencyList[first]) {
            if(i.first==second && i.second<weight){
                weight=i.second;
            } 
        }
        return weight;
    }

    const std::vector<std::pair<std::pair<uint32_t, uint32_t>, N>> getListOfEdges() const override {
        std::vector<std::pair<std::pair<uint32_t,uint32_t>,N>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(auto j: adjacencyList[i]) {
                if(i<=j.first) {
                    result.push_back(std::pair<std::pair<uint32_t,uint32_t>,N>(std::pair<uint32_t,uint32_t>(i,j.first),j.second));
                }
            }
        }
        return result;
    }
};

class ListGraph : public GraphUnweighted{
private:
    std::vector<std::vector<uint32_t>> adjacencyList;
public:
    ListGraph(const uint32_t size){
        for(uint32_t i=0;i<size;++i) adjacencyList.push_back(std::vector<uint32_t>());
        this->is_directed=false;
        this->is_weighted=false;
    }

    ~ListGraph()=default;

    uint32_t getSize() const override{
        return adjacencyList.size();
    }

    bool areVertexesConnected(uint32_t first, uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(uint32_t, uint32_t) in a class ListGraph is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(uint32_t, uint32_t) in a class ListGraph is out of bounds.");
        }
        if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first, second);
        for(uint32_t i : adjacencyList[first]) {
            if(i==second) return true;
        }
        return false;
    }

    uint32_t addVertex() override{
        adjacencyList.push_back(std::vector<uint32_t>());
        return adjacencyList.size()-1;
    }

    void deleteEdge(uint32_t first, uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(uint32_t, uint32_t) in a class ListGraph is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(uint32_t, uint32_t) in a class ListGraph is out of bounds.");
        }
        if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first, second);
        bool flag=false;

        for(int32_t i=0;i<adjacencyList[first].size();++i){
            if(adjacencyList[first][i]==second){
                adjacencyList[first].erase(adjacencyList[first].begin()+i);
                flag=true;
                --i;
            }
        }

        if(flag){
            for(int32_t i=0;i<adjacencyList[second].size();++i) {
                if(adjacencyList[second][i]==first) {
                    adjacencyList[second].erase(adjacencyList[second].begin()+i);
                    --i;
                }
            }
        }
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        return adjacencyList[vertex];
    }

    void addEdge(uint32_t first, uint32_t second) override{
        this->addEdge(first, second, false);
    }

    void addEdge(uint32_t first, uint32_t second, const bool saveFlag){
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(uint32_t, uint32_t) in a class ListGraph is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(uint32_t, uint32_t) in a class ListGraph is out of bounds.");
        }
        bool flag=false;
        if(saveFlag){
            if(adjacencyList[first].size()>adjacencyList[second].size()) std::swap(first,second);
            for(auto i : adjacencyList[first]){
                if(i==second){
                    flag=true;
                    break;
                }
            }
        }
        
        if(!flag){
            adjacencyList[first].push_back(second);
            if(first!=second){
                adjacencyList[second].push_back(first);
            }
        }
    }

    const std::vector<std::pair<uint32_t, uint32_t>> getListOfEdges() const override {
        std::vector<std::pair<uint32_t, uint32_t>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(auto j : adjacencyList[i]) {
                if(i<=j) {
                    result.push_back(std::pair<uint32_t, uint32_t>(i,j));
                }
            }
        }
        return result;
    }
};

class MatrixGraph : public GraphUnweighted{
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
public:
    MatrixGraph(const uint32_t size){
        for(uint32_t i=0;i<size;++i) adjacencyMatrix.push_back(std::vector<bool>(i+1));
        this->is_directed=false;
        this->is_weighted=false;
    }

    ~MatrixGraph()=default;

    uint32_t getSize() const override{
        return adjacencyMatrix.size();
    }

    bool areVertexesConnected(uint32_t first, uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(uint32_t, uint32_t) in a class MatrixGraph is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(uint32_t, uint32_t) in a class MatrixGraph is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        return adjacencyMatrix[first][second];
    }

    uint32_t addVertex() override{
        uint32_t size=adjacencyMatrix.size();
        adjacencyMatrix.push_back(std::vector<bool>(size));
        return size;
    }

    void deleteEdge(uint32_t first, uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(uint32_t, uint32_t) in a class MatrixGraph is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(uint32_t, uint32_t) in a class MatrixGraph is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        adjacencyMatrix[first][second]=false;
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class MatrixGraph is out of bounds.");
        }
        std::vector<uint32_t>result;
        uint32_t i=0;
        for(;i<=vertex;++i) {
            if(adjacencyMatrix[vertex][i]) result.push_back(i);
        }
        for(;i<adjacencyMatrix.size();++i) {
            if(adjacencyMatrix[i][vertex]) result.push_back(i);
        }
        return result;
    }

    void addEdge(uint32_t first, uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(uint32_t, uint32_t) in a class MatrixGraph is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(uint32_t, uint32_t) in a class MatrixGraph is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        adjacencyMatrix[first][second]=true;
    }

    const std::vector<std::pair<uint32_t, uint32_t>> getListOfEdges() const override {
        std::vector<std::pair<uint32_t, uint32_t>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(uint32_t j=0;j<i+1;++j) {
                if(adjacencyMatrix[i][j]) result.push_back(std::pair<uint32_t, uint32_t>(i,j));
            }
        }
        return result;
    }
};

class MatrixGraphDirected : public GraphUnweighted{
private:
    std::vector<std::vector<bool>> adjacencyMatrix;
public:
    MatrixGraphDirected(const uint32_t size){
        for(uint32_t i=0;i<size;++i) adjacencyMatrix.push_back(std::vector<bool>(size, false));
        this->is_weighted=false;
        this->is_directed=true;
    }

    ~MatrixGraphDirected()=default;

    void transpose(){
        const uint32_t size=this->getSize();
        bool tmp;
        for(uint32_t i=0;i<size;++i){
            for(uint32_t j=i+1;j<size;++j){
                tmp=this->adjacencyMatrix[i][j];
                this->adjacencyMatrix[i][j]=this->adjacencyMatrix[j][i];
                this->adjacencyMatrix[j][i]=tmp;
            }
        }
    }

    uint32_t getSize() const override{
        return adjacencyMatrix.size();
    }

    bool areVertexesConnected(const uint32_t first, const uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        return adjacencyMatrix[first][second];
    }

    uint32_t addVertex() override{
        const uint32_t size=adjacencyMatrix.size();
        for(uint32_t i=0;i<size;++i) adjacencyMatrix[i].push_back(false);
        adjacencyMatrix.push_back(std::vector<bool>(size+1));
        return size;
    }

    void deleteEdge(const uint32_t first, const uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        adjacencyMatrix[first][second]=false;
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        std::vector<uint32_t>result;
        for(uint32_t i=0;i<adjacencyMatrix[vertex].size();++i) {
            if(adjacencyMatrix[vertex][i]) result.push_back(i);
        }
        return result;
    }

    void addEdge(const uint32_t first, const uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(const uint32_t, const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(const uint32_t, const uint32_t) in a class MatrixGraphDirected is out of bounds.");
        }
        adjacencyMatrix[first][second]=true;
    }

    const std::vector<std::pair<uint32_t, uint32_t>> getListOfEdges() const override {
        std::vector<std::pair<uint32_t,uint32_t>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(uint32_t j=0;j<getSize();++j) {
                if(adjacencyMatrix[i][j]) result.push_back(std::pair<uint32_t, uint32_t>(i,j));
            }
        }
        return result;
    }
};

template <typename N>
class MatrixGraphWeightedAndDirected : public WeightedGraph<N>{
private:
    std::vector<std::vector<N>> adjacencyMatrix;
public:
    MatrixGraphWeightedAndDirected(const uint32_t size, const N &max){
        this->max=max;
        for(uint32_t i=0;i<size;++i) adjacencyMatrix.push_back(std::vector<N>(size, max));
        this->is_weighted=true;
        this->is_directed=true;
    }

    ~MatrixGraphWeightedAndDirected() = default;

    void transpose(){
        uint32_t size=this->getSize();
        for(uint32_t i=0;i<size;++i){
            for(uint32_t j=i+1;j<size;++j){
                std::swap(this->adjacencyMatrix[i][j], this->adjacencyMatrix[j][i]);
            }
        }
    }

    uint32_t getSize() const override{
        return adjacencyMatrix.size();
    }

    bool areVertexesConnected(const uint32_t first, const uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(const uint32_t, const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        return adjacencyMatrix[first][second]!=this->max;
    }

    uint32_t addVertex() override{
        const uint32_t size=adjacencyMatrix.size();
        for(uint32_t i=0;i<size;++i) {
            adjacencyMatrix[i].push_back(this->max);
        }
        adjacencyMatrix.push_back(std::vector<N>(size+1, this->max));
        return size;
    }

    void deleteEdge(const uint32_t first, const uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(const uint32_t, const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(const uint32_t, const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        adjacencyMatrix[first][second]=this->max;
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        std::vector<uint32_t>result;
        for(uint32_t i=0;i<adjacencyMatrix[vertex].size();++i) {
            if(adjacencyMatrix[vertex][i]!=this->max) result.push_back(i);
        }
        return result;
    }

    const std::vector<std::pair<uint32_t, N>>getNeighboursWithWeights(const uint32_t vertex) const override {
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighboursWithWeights(const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        std::vector<std::pair<uint32_t, N>>result;
        for(uint32_t i=0;i<adjacencyMatrix[vertex].size();++i) {
            if(adjacencyMatrix[vertex][i]!=this->max) result.push_back(std::pair<uint32_t,N>(i,adjacencyMatrix[vertex][i]));
        }
        return result;
    }

    void addEdge(const uint32_t first, const uint32_t second, const N &weight) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(const uint32_t, const uint32_t, const N) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(const uint32_t, const uint32_t, const N) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        adjacencyMatrix[first][second]=weight;
    }

    N getWeight(const uint32_t first, const uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method getWeight(const uint32_t, const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method getWeight(const uint32_t, const uint32_t) in a class MatrixGraphWeightedAndDirected<N> is out of bounds.");
        }
        return adjacencyMatrix[first][second];
    }

    const std::vector<std::pair<std::pair<uint32_t, uint32_t>, N>> getListOfEdges() const override {
        std::vector<std::pair<std::pair<uint32_t,uint32_t>,N>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(uint32_t j=0;j<getSize();++j) {
                if(adjacencyMatrix[i][j]!=this->max) {
                    result.push_back(std::pair<std::pair<uint32_t,uint32_t>,N>(std::pair<uint32_t,uint32_t>(i,j),adjacencyMatrix[i][j]));
                }
            }
        }
        return result;
    }
};

template <typename N>
class MatrixGraphWeighted : public WeightedGraph<N>{
private:
    std::vector<std::vector<N>> adjacencyMatrix;
public:
    MatrixGraphWeighted(const uint32_t size, const N &max){
        this->max=max;
        for(uint32_t i=0;i<size;++i) adjacencyMatrix.push_back(std::vector<N>(i+1, max));
        this->is_weighted=true;
        this->is_directed=false;
    }

    ~MatrixGraphWeighted()=default;

    uint32_t getSize() const override{
        return adjacencyMatrix.size();
    }

    bool areVertexesConnected(uint32_t first, uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method areVertexesConnected(uint32_t, uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method areVertexesConnected(uint32_t, uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        return adjacencyMatrix[first][second]!=this->max;
    }

    uint32_t addVertex() override{
        const uint32_t size=adjacencyMatrix.size();
        for(uint32_t i=0;i<size;++i) adjacencyMatrix[i].push_back(this->max);
        adjacencyMatrix.push_back(std::vector<N>(size+1, this->max));
        return size;
    }

    void deleteEdge(uint32_t first, uint32_t second) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method deleteEdge(uint32_t, uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method deleteEdge(uint32_t, uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        adjacencyMatrix[first][second]=this->max;
    }

    const std::vector<uint32_t> getNeighbours(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighbours(const uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        uint32_t i=0;
        std::vector<uint32_t>result;
        for(;i<=vertex;++i) {
            if(adjacencyMatrix[vertex][i]!=this->max) result.push_back(i);
        }
        for(;i<adjacencyMatrix.size();++i) {
            if(adjacencyMatrix[i][vertex]!=this->max) result.push_back(i);
        }
        return result;
    }

    const std::vector<std::pair<uint32_t, N>> getNeighboursWithWeights(const uint32_t vertex) const override{
        if(this->isIndexOutOfBounds(vertex)){
            printErrorMsg(2, "The argument of a method getNeighboursWithWeights(const uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        uint32_t i=0;
        std::vector<std::pair<uint32_t, N>>result;
        for(;i<=vertex;++i) {
            if(adjacencyMatrix[vertex][i]!=this->max) result.push_back(std::pair<uint32_t,N>(i, adjacencyMatrix[vertex][i]));
        }
        for(;i<adjacencyMatrix.size();++i) {
            if(adjacencyMatrix[i][vertex]!=this->max) result.push_back(std::pair<uint32_t,N>(i, adjacencyMatrix[i][vertex]));
        }
        return result;
    }

    void addEdge(uint32_t first, uint32_t second, const N &weight) override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method addEdge(uint32_t, uint32_t, const N) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method addEdge(uint32_t, uint32_t, const N) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        adjacencyMatrix[first][second]=weight;
    }

    N getWeight(uint32_t first, uint32_t second) const override{
        if(this->isIndexOutOfBounds(first)){
            printErrorMsg(2, "The first argument of a method getWeight(uint32_t, uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(this->isIndexOutOfBounds(second)){
            printErrorMsg(2, "The second argument of a method getWeight(uint32_t, uint32_t) in a class MatrixGraphWeighted<N> is out of bounds.");
        }
        if(first<second) std::swap(first, second);
        return adjacencyMatrix[first][second];
    }

    const std::vector<std::pair<std::pair<uint32_t, uint32_t>, N>> getListOfEdges() const override {
        std::vector<std::pair<std::pair<uint32_t,uint32_t>,N>>result;
        for(uint32_t i=0;i<getSize();++i) {
            for(uint32_t j=0;j<i+1;++j) {
                if(adjacencyMatrix[i][j]!=this->max) {
                    result.push_back(std::pair<std::pair<uint32_t,uint32_t>,N>(std::pair<uint32_t,uint32_t>(i,j),adjacencyMatrix[i][j]));
                }
            }
        }
        return result;
    }
};

#endif
