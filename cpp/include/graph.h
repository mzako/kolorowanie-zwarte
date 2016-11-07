#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
#include "edge.h"

class Graph {
public:
    Graph(std::string fileName);

    const std::map<int, std::vector<Edge>>& getAdj() const;
    void serialize(std::string fileName) const;

private:
    void deserialize(std::string fileName);

    std::map<int, std::vector<Edge>> adj;
};
#endif //GRAPH_H
