/**
 *  @author Michal Kostrzewa
 *  @author Michal Zakowski
 */

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

extern bool verbose;

/**
 * Main class of a graph
 */
class Graph {
public:
    /**
     * Constructor.
     * Reas graph from file and fill adjacency list.
     */
    Graph(std::string fileName);

    /**
     * Constructor.
     * Initialize graph from adjacency list.
     */
    Graph(AdjList& a);

    /**
     * Return adjacency list
     */
    const AdjList& getAdj() const;

    /**
     * Add edge to the graph.
     */
    void addEdge(const Edge& e);

    /**
     * Serialize graph to .dot (graphviz) and .txt (raw) formats.
     */
    void serialize(std::string fileName) const;

    /**
     * Constructor.
     * Read graph from file and fills adjacency list.
     */
    std::vector<Edge*> pathEdges(const std::vector<int>& elem);

    /**
     * Recursively color path in graph.
     */
    bool colorPath(std::vector<Edge*> edges);
    /**
     * Clears given path (set color 0) in graph.
     */
    void zeroPath(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end);
    /**
     * Determines possible coloring for given vertex considering current constraints.
     */
    std::vector<int> legalColoringsOf(const int vertexIndex) const;
    /**
     * Color edge if exists in graph.
     */
    void colorEdge(const int v1, const int v2, const int color);
    /**
     * Return edge adjacent to v1 and v2.
     */
    Edge& getEdge(const int v1, const int v2);
    /**
     * Check if interval of coloring of edges adjacent to vertexIndex doesn't have gaps
     */
    bool areGaps(const int vertexIndex) const;
    /**
     * Return lowest color in all vertexIndex constraints
     */
    int getLowestColor(const int vertexIndex) const;
    /**
     * Return highest color in all vertexIndex constraints
     */
    int getHighestColor(const int vertexIndex) const;
    /**
     * Recursively search for cycle in the graph.
     */
    std::vector<int> findCycle();
    /**
     * Color this graph assuming it is forest considering vertex constraints.
     */
    bool colorAsForest();

    std::vector<int> legalColoringsOfEdge(const int v1, const int v2) const;
    /**
     * Move single edge from this graph to other including constraints on vertices v1 and v2.
     */
    void moveEdgeToAnotherGraph(Graph& other, const int v1, const int v2);
    /**
     * Move all edges from this graph to other including constraints on vertices.
     */
    void moveAllEdgesToAnotherGraph(Graph& other);
    /**
     * Move all hanging edges from this graph to outGraph.
     * Return true if at least one edge was moved. False otherwise.
     */
    bool moveHangingEdgesTo(Graph& outGraph);
    /**
     * Main function for graph coloring.
     * Return true if graph can be consecutive colored. False otherwise.
     */
    bool color(Graph& outGraph);
    /**
     * Print this graph including constraints.
     */
    void print() const;
    /**
     * Check if edges adjacent to given vertex are consecutive colored
     */
    bool isOK(const int vertexIndex);
    /**
     * Check if edge containing given vertices exists.
     */
    bool isEdge(const int v1, const int v2);
    /**
     * Add single constraint to set of constraints for vertexIndex
     */
    void addVertexConstraint(const int vertexIndex, const int color);
    /**
     * Return all constraints for single vertex.
     * Constraints are real colorings of edges plus artificial constraints from constraints map
     */
    std::vector<int> getAllVertexConstraints(const int vertexIndex) const;
    /**
    * Return all vertices in given cycle that have at least one constraint set.
    */
    std::vector<int> findConstrainedVerticesInCycle(std::vector<int> indices) const;
    /**
    * Split cycle to number of paths using cutVertices.
    */
    std::vector<std::vector<int> > splitCycle(std::vector<int> cycle, 
        std::vector<int> cutVertices) const;
    /**
    * Return number of edges in graph.
    */
    int numEdges() const;

    /**
     * Recursively find path in graph.
     */
    std::vector<int> findPath();
private:
    /**
     * Read graph from file.
     */
    void deserialize(std::string fileName);
    /**
     * Recursively find path in graph.
     */
    bool colorPathRecur(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end);
    /**
     * Recursively color cycle.
     */
    std::vector<int> findCycleRecur(const int startingVertexIdx, 
        const int currentVertexIdx, const int prevIdx);
    /**
     * Return first hanging edge in graph.
     * Hanging edge is adjacent to vertices that have adjacency list of size 1.
     */
    Edge* findHangingEdge();
    /**
     * Print this, temp and out graphs only if in verbose mode.
     */
    void printGraphs(const Graph& temp, const Graph& out) const;
    /**
     * Recursively find path in graph.
     */
    std::vector<int> findPathRecur(const int startingVertexIdx, 
        const int currentVertexIdx, const bool mustEndWithConstrained);

    /**
     * Adjacency lists for all vertices in graph.
     */
    AdjList adj;
    /**
     * Map of logical values for every vertex detemining if it was visited in cycle or path search.
     */
    VertexLabels labels;
    /**
     * Color constraints put on each vertex in graph.
     */
    VertexConstraints constraints;
};
#endif //GRAPH_H
