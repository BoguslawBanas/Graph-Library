#include <stdio.h>
#include <time.h>
#include "../graph_library.h"

int main(){
    srand(time(0)); //function needed to generate pseudo-random numbers.
    ListGraph listgraph(10); //creating an instance of ListGraph class, that contains 10 vertexes <0;9>.
    uint32_t a,b;

    for(int i=0;i<12;++i){
        //generating two random numbers <0;9>
        a=rand()%10;
        b=rand()%10;

        //adding edge to the graph (if edge between two vertexes already exists we skip adding another one).
        listgraph.addEdge(a,b,true);
    }

    //dynamically creating an instance of MatrixGraph class, that contains 10 vertexes <0;9>
    MatrixGraph *matrixgraph=new MatrixGraph(10);

    for(int i=0;i<12;++i){
        a=rand()%10;
        b=rand()%10;

        //checking if there is already an edge between two vertexes.
        while(matrixgraph->areVertexesConnected(a,b)){
            a=rand()%10;
            b=rand()%10;
        }
        //adding edge
        matrixgraph->addEdge(a,b);
    }

    //releasing memory
    delete matrixgraph;
    return 0;
}