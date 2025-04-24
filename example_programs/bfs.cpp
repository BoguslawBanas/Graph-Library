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

    //calling bfs function that returns distance between vertex number 0 and vertex number 5.
    int32_t result=bfs<ListGraph>(graph, 0, 5);

    std::cout<<result<<'\n';

    //creating an instance of ExampleClass.
    ExampleClass exampleClass;

    //calling bfs function that returns distance between vertex number 0 and vertex number 5, during exploring vertexes we call operator that clarifies moving through the graph.
    result=bfs_with_f<ListGraph, ExampleClass>(graph, 0, 5, exampleClass);

    std::cout<<result<<'\n';
    return 0;
}
