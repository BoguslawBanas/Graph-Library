//Program to color graph using bfs_functor().

#include <iostream>
#include <map>
#include "../graph_library.h"

#define MAX_COLORS 8

#include <iostream>
#include <map>
#include <string>

class Functor{
private:
    ListGraph *graph;
    std::vector<int32_t>v;
public:
    Functor(const uint32_t size, ListGraph *graph){
        this->v=std::vector<int32_t>(size, -1);
        this->graph=graph;
    }

    ~Functor()=default;

    const std::vector<int32_t>& getVector(){
        return this->v;
    }

    void operator()(const uint32_t vertex){
        auto neighboors=this->graph->getNeighbours(vertex);

        std::vector<bool> usedColors(MAX_COLORS, false);
        for(auto &it : neighboors){
            if(this->v[it]!=-1){
                usedColors[this->v[it]]=true;
            }
        }

        uint32_t i=0;

        while(i<MAX_COLORS && usedColors[i]){
            ++i;
        }
        this->v[vertex]=i;
    }
};

int main(){
    std::map<int, std::string> colorMap;

    colorMap[0] = "Red";
    colorMap[1] = "Green";
    colorMap[2] = "Blue";
    colorMap[3] = "Yellow";
    colorMap[4] = "Purple";
    colorMap[5] = "Orange";
    colorMap[6] = "Black";
    colorMap[7] = "White";

    uint32_t size_of_graph=6;
    ListGraph listgraph(size_of_graph);
    Functor func(size_of_graph, &listgraph);

    listgraph.addEdge(0,1);
    listgraph.addEdge(0,2);
    listgraph.addEdge(2,3);
    listgraph.addEdge(2,4);
    listgraph.addEdge(2,5);
    listgraph.addEdge(3,5);

    bfs_functor<ListGraph, Functor>(listgraph, 0, func);

    auto result=func.getVector();
    for(int32_t i=0;i<result.size();++i){
        std::cout<<i<<' '<<colorMap[result[i]]<<'\n';
    }
    return 0;
}
