#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>

#include "edge.h"

using AdjList = std::map<int, std::vector<Edge>>;

class Graph {
public:
    Graph(std::string fileName);
    Graph(AdjList& a);

    const AdjList& getAdj() const;
    void addEdge(const Edge& e);
    void serialize(std::string fileName) const;

    std::vector<Edge*> pathEdges(const std::vector<int>& elem);
    bool colorPath(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end);
    void zeroPath(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end);
    std::vector<int> legalColoringsOf(const int vertexIndex) const;
    void colorEdge(const int v1, const int v2, const int color);
    Edge& getEdge(const int v1, const int v2);
    bool areGaps(const int vertexIndex) const;
    int getLowestColor(const int vertexIndex) const;
    int getHighestColor(const int vertexIndex) const;
private:
    void deserialize(std::string fileName);


    AdjList adj;
};
#endif //GRAPH_H