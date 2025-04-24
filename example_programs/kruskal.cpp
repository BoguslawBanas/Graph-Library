#include <iostream>
#include "../graph_library.h"

#define INF 10000000

int main(){
    //creating an instance of MatrixGraphWeighted.
    MatrixGraphWeighted<int> graph(7, INF);

    //adding edges
    graph.addEdge(0, 1, 20);
    graph.addEdge(0, 2, 30);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 4, 20);
    graph.addEdge(2, 5, 35);
    graph.addEdge(2, 6, 50);
    graph.addEdge(3, 4, 5);
    graph.addEdge(3, 5, 10);
    graph.addEdge(4, 6, 20);
    graph.addEdge(5, 6, 15);

    //calling function that creates minimal spanning tree (Kruskal algorithm).
    auto mst=kruskal<MatrixGraphWeighted<int>, int, Disjoint_Set>(graph);

    if(mst){ //checking if result is not equal nullptr.
        for(auto &it : *mst){ //if result is not equal nullptr, that means mst was built correctly.
            std::cout<<it.first<<' '<<it.second<<' '<<graph.getWeight(it.first, it.second)<<'\n';
        }
    }
    else{//if result is equal nullptr, that means mst was not built (for instance there can be an isolated vertex in graph).
        std::cout<<"Unable to create a minimal spanning tree from a given graph.\n";
    }
}