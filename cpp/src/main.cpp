#include <iostream>
#include "../include/graph.h"

using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 3)
        cout<<"usage: "<< argv[0] <<" <input file> <output file>" << endl;
    else {
        Graph graph(argv[1]);
        graph.serialize(argv[2]);
    }

    return 0;
}