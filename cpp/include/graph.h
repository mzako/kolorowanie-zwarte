#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <set>

#include "edge.h"

using AdjList = std::map<int, std::vector<Edge>>;
using VertexLabels = std::map<int, bool>;
using VertexConstraints = std::map<int, std::set<int>>;

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
    std::vector<int> findCycle();
    void colorAsForest();
    std::vector<int> legalColoringsOfEdge(const int v1, const int v2) const;
    void moveEdgeToAnotherGraph(Graph& other, const int v1, const int v2);
    void moveAllEdgesToAnotherGraph(Graph& other);
    bool moveHangingEdgesTo(Graph& outGraph);
    void color(Graph& outGraph);
    void print() const;
    bool isOK(const int vertexIndex);
    bool isEdge(const int v1, const int v2);
    void addVertexConstraint(const int vertexIndex, const int color);
    std::vector<int> getAllVertexConstraints(const int vertexIndex) const;
private:
    void deserialize(std::string fileName);

    std::vector<int> findCycleRecur(const int startingVertexIdx, 
        const int currentVertexIdx, const int prevIdx);
    Edge* findHangingEdge();
    void printGraphs(const Graph& temp, const Graph& out) const;

    AdjList adj;
    VertexLabels labels;
    VertexConstraints constraints;
};
#endif //GRAPH_H
