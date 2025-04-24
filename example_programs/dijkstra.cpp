#include <iostream>
#include "../graph_library.h"

class ExampleClass{
public:
    ExampleClass(){}

    ~ExampleClass()=default;

    bool operator()(const uint32_t vertex, const float distance) const{
        if(vertex==5){
            return false;
        }
    }
};

int main(){
    float inf=1/0.f; //define infinity for the graph.
    ListGraphWeightedAndDirected<float>graph(7, inf); //creating an instance of a graph.

    //adding some edges
    graph.addEdge(0, 1, 15.5f);
    graph.addEdge(1, 3, 42.3f);
    graph.addEdge(3, 4, 9.45f);
    graph.addEdge(4, 5, 32.4f);
    graph.addEdge(5, 6, 11.f);
    graph.addEdge(1, 2, 104.f);
    graph.addEdge(2, 6, 18.88f);

    //calling dijkstra algorithm to find shortest distance between vertex 0 and 6.
    std::cout<<dijkstra<ListGraphWeightedAndDirected<float>, float, Priority_Q<BinaryHeap<float>, float>>(graph, 0, 6)<<'\n';

    //calling dijkstra algorithm to find shortest distance between vertex 0 and 6 with special conditions that are written in ExampleClass.
    std::cout<<dijkstra_with_f<ListGraphWeightedAndDirected<float>, float, Priority_Q<FibonacciHeap<float>, float>>(graph, 0, 6, ExampleClass())<<'\n';

    return 0;
}