/**
 *  @author Michal Kostrzewa
 *  @author Michal Zakowski
 */

#include <iostream>
#include "../include/graph.h"

/**
 * Starting point of the program
 */
int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 5)
        std::cout<<"usage: "<< argv[0] <<" <input file> <output file> [--dontcolor] [--verbose]" << std::endl;
    else {

        bool dontcolor = false;
        std::vector<std::string> flags;
        if(argc == 4) {
            flags.emplace_back(std::string(argv[3]));
        }
        if(argc == 5) {
            flags.emplace_back(std::string(argv[3]));
            flags.emplace_back(std::string(argv[4]));
        }

        for(const std::string flag : flags) {
            if(flag == "--dontcolor") {
                dontcolor = true;
            } else if(flag == "--verbose") {
                verbose = true;
            } else {
                std::cout << "Invalid flag";
                return 1;
            }
        }

        Graph graph(argv[1]);

        if(dontcolor) {
            graph.serialize(argv[2]);
        } else {
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
    }

    return 0;
}