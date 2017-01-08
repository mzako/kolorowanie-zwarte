#include <iostream>
#include "../include/graph.h"

using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 3)
        cout<<"usage: "<< argv[0] <<" <input file> <output file>" << endl;
    else {
        Graph graph(argv[1]);
        AdjList a;
        auto outGraph = Graph(a);
        const bool success = graph.color(outGraph);
        if(!success) {
            std::cout << std::endl << " ~~~~~~ FAILED TO COLOR GRAPH :( ~~~~~~ " 
                      << std::endl;
        } else {
            std::cout << std::endl << " ~~~~~~ SUCCESS :) ~~~~~~ " << std::endl;
        }
        outGraph.print();
        outGraph.serialize(argv[2]);
    }

    return 0;
}