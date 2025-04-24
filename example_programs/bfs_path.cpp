#include <iostream>
#include "../graph_library.h"

class ExampleClass{
public:
    ExampleClass(){}

    ~ExampleClass()=default;

    bool operator()(const uint32_t vertex, const int32_t distance) const{
        if(vertex==2 || (vertex==4 && distance>15)){
            return false;
        }
        return true;
    }
};

int main(){
    ListGraph graph(7); //creating an instance of ListGraph class with 6 vertexes (0-5).

    //adding some edges
    graph.addEdge(0,1);
    graph.addEdge(0,2);
    graph.addEdge(1,3);
    graph.addEdge(1,6);
    graph.addEdge(2,5);
    graph.addEdge(3,4);
    graph.addEdge(4,5);

    auto result=bfs_path<ListGraph>(graph, 0, 5);

    if(result){
        for(auto &i : *result){
            std::cout<<i<<' ';
        }
        std::cout<<'\n';
    }

    delete result;
    result=nullptr;

    ExampleClass exampleClass;

    auto result2=bfs_path_with_f<ListGraph, ExampleClass>(graph, 0, 5, exampleClass);

    if(result2){
        for(auto &i : *result2){
            std::cout<<i<<' ';
        }
        std::cout<<'\n';
    }

    delete result2;
    result2=nullptr;
    return 0;
}
