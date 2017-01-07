#include "../include/graph.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>

#define LOGLEVEL 2

void log(int level, std::string msg) {
    if(level >= LOGLEVEL) {
        std::cout << msg;
    }
}

std::ostream& operator<< (std::ostream& os, const Graph& graph) {
    for (auto& kv : graph.getAdj()) {
        os << kv.first << " ";

        for (auto& neighbour : kv.second) {
            os << neighbour.v2 << " ";
        }

        os << std::endl;
    }

    return os;
}

Graph::Graph(std::string fileName) {
    deserialize(fileName);
}

Graph::Graph(AdjList& a) {
    adj = a;
}

void Graph::deserialize(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        int vertex, neighbour;

        iss >> vertex;
        adj[vertex] = std::vector<Edge>();
        while (iss >> neighbour) {
            adj[vertex].emplace_back(vertex, neighbour);
        }
    }
}

void Graph::serialize(std::string fileName) const {
    std::ofstream file(fileName);

    if (file) {
        file << "graph {" << std::endl;

        for (auto& kv : adj) {
            for (auto& neighbour : kv.second) {
                if(neighbour.v1 <= neighbour.v2) {
                    file << "    " << neighbour.v1 << " -- " << neighbour.v2
                         << " [label=" << neighbour.color << "]" << std::endl;
                }
            }
        }

        file << "}" << std::endl;
    }
}

const std::map<int, std::vector<Edge>>& Graph::getAdj() const {
    return adj;
}

void Graph::addEdge(const Edge& e) {
    adj[e.v1].emplace_back(e.v1, e.v2, e.color);
    adj[e.v2].emplace_back(e.v2, e.v1, e.color);
}

std::vector<Edge*> Graph::pathEdges(const std::vector<int>& elem) {

    std::vector<Edge*> edges;
    for(size_t i = 0; i < elem.size()-1; i++) {
        int currentVertex = elem[i], nextVertex = elem[i+1];
        for(size_t j = 0; j < adj.at(currentVertex).size(); j++) {
            if(adj.at(currentVertex)[j].v2 == nextVertex) {
                edges.emplace_back(&adj.at(currentVertex)[j]);
                break;
            }
        }
    }
    // loop around
    int lastVertex = elem.back(), firstVertex = elem[0];
    for(size_t j = 0; j < adj.at(lastVertex).size(); j++) {
        if(adj.at(lastVertex)[j].v2 == firstVertex) {
            edges.emplace_back(&adj.at(lastVertex)[j]);
            break;
        }
    }
    return edges;
}

bool Graph::colorPath(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end) {

    // looped around?
    if(edge == end) {
        std::cout << "BACKTRACK COLORING SUCCESSFUL" << std::endl;
        return true;
    }

    const int currentVertexIdx = (*edge)->v1, nextVertexIdx = (*edge)->v2;

    const std::vector<int> legalsOfEdge = legalColoringsOfEdge(currentVertexIdx, 
        nextVertexIdx);

    for(const int currentColor : legalsOfEdge) {
        colorEdge(currentVertexIdx, nextVertexIdx, currentColor);
        if(colorPath(++edge, end)) {
            // see if our colors are fine
            if(!areGaps(currentVertexIdx)) {
                return true;
            } else {
                // zero path ahead of us
                --edge;
                zeroPath(++edge, end);
            }
        }
        --edge;
    }
    std::cout << "FAILED VERTEX " << currentVertexIdx << std::endl;
    colorEdge(currentVertexIdx, nextVertexIdx, 0);
    return false;
}

void Graph::zeroPath(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end) {
    if(edge == end) {
        return;
    }
    colorEdge((*edge)->v1, (*edge)->v2, 0);
    zeroPath(++edge, end);
}

std::vector<int> Graph::legalColoringsOf(const int vertexIndex) const {
    std::vector<int> possibleColors;

    if(areGaps(vertexIndex)) {
        // there's a gap: find it and return it
        std::vector<int> colors = getAllVertexConstraints(vertexIndex);
        std::sort(colors.begin(), colors.end());
        for(int i = 0; i < colors.size()-1; i++) {
            if(colors[i+1] - colors[i] != 1) {
                possibleColors.emplace_back((colors[i+1] + colors[i]) / 2);
                break;
            }
        }
    } else {
        const int lowestColor = getLowestColor(vertexIndex),
            highestColor = getHighestColor(vertexIndex);

        if(lowestColor == -1) {
            return possibleColors; // any color is fine
        }
        if(lowestColor > 1) {
            possibleColors.emplace_back(lowestColor-1);
            if(lowestColor > 2) {
                possibleColors.emplace_back(lowestColor-2);
            }
        }
        if(highestColor != -1) {
            possibleColors.emplace_back(highestColor+1);
            possibleColors.emplace_back(highestColor+2);
        } else if(highestColor == -1) {
            possibleColors.emplace_back(1);
        }
    }

    return possibleColors;
}

void Graph::colorEdge(const int v1, const int v2, const int color) {
    std::cout << "COLORING EDGE " << v1 << " " << v2 << " WITH " << color << std::endl;
    for(auto& edge : adj.at(v1)) {
        if((edge.v1 == v1 && edge.v2 == v2) || (edge.v2 == v1 && edge.v1 == v2)) {
            edge.color = color;
        }
    }
    for(auto& edge : adj.at(v2)) {
        if((edge.v1 == v1 && edge.v2 == v2) || (edge.v2 == v1 && edge.v1 == v2)) {
            edge.color = color;
        }
    }
}

Edge& Graph::getEdge(const int v1, const int v2) {
    for(auto& edge : adj.at(v1)) {
        if((edge.v1 == v1 && edge.v2 == v2) || (edge.v2 == v1 && edge.v1 == v2)) {
            return edge;
        }
    }
}

bool Graph::areGaps(const int vertexIndex) const {
    const int lowestColor = getLowestColor(vertexIndex),
        highestColor = getHighestColor(vertexIndex);
    int sum = 0; 
    for(const int c : getAllVertexConstraints(vertexIndex)) {
        sum += c;
    }
    const int expectedSum = (highestColor * (highestColor + 1) - 
        lowestColor * (lowestColor + 1) ) / 2;
    return sum < expectedSum;
}

int Graph::getLowestColor(const int vertexIndex) const {
    int lowestColor = -1;
    for(const int c : getAllVertexConstraints(vertexIndex)) {
        if(lowestColor == -1) {
            // first non-zero colour found
            lowestColor = c;
        }
        if(c < lowestColor) {
            lowestColor = c;
        }
    }
    return lowestColor;
}

int Graph::getHighestColor(const int vertexIndex) const {
    int highestColor = -1;
    for(const int c : getAllVertexConstraints(vertexIndex)) {
        if(highestColor == -1) {
            // first non-zero colour found
            highestColor = c;
        }
        if(c > highestColor) {
            highestColor = c;
        }
    }
    return highestColor;
}

std::vector<int> Graph::findCycle() {
    // cleanup labels
    labels.clear();
    for(auto& keyval : adj) {
        labels[keyval.first] = false;
    }

    const int startingVertexIdx = labels.begin()->first;

    auto result = findCycleRecur(startingVertexIdx, startingVertexIdx, startingVertexIdx);

    if(result.size()) {
        std::reverse(result.begin(), result.end());
        result.emplace_back(startingVertexIdx);

        std::cout << "LOOP FOUND: ";
        for(const auto& el : result) {
            std::cout << el << ", ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "NO LOOP FOUND" << std::endl;
    }

    // cleanup labels
    for(auto& keyval : labels) {
        keyval.second = false;
    }

    return result;
}

std::vector<int> Graph::findCycleRecur(const int startingVertexIdx, 
    const int currentVertexIdx, const int prevIdx) {
    labels[currentVertexIdx] = true;

    for(auto& edge : adj.at(currentVertexIdx)) {
        const int neighbourIdx = edge.v2;
        if(neighbourIdx == prevIdx) {
            continue;
        }

        // loop found right now?
        if(neighbourIdx == startingVertexIdx) {
            return std::vector<int>{currentVertexIdx};
        }
        
        if(labels.at(neighbourIdx) == false) {

            auto result = findCycleRecur(startingVertexIdx, edge.v2, currentVertexIdx);

            // loop found by someone we called?
            if(!result.empty()) {
                result.emplace_back(currentVertexIdx);
                return result;
            }
        }
    }

    return std::vector<int>{}; // return empty
}

void Graph::colorAsForest() {
    int numUncolored = 0;
    for(auto& v : adj) {
        for(auto& e : v.second) {
            numUncolored++;
        }
    }
    numUncolored /= 2; // each edge is counted twice
    std::cout << "COLORING A FOREST WITH " << numUncolored << " EDGES" << std::endl;

    while(numUncolored != 0) {
        int numColoredInThisTree = 0;
        std::set<int> setK;

        int startV1, startV2, startColor;

        // first, look for an edge with a constraints
        for(const auto& c : constraints) {
            for(const auto& v : adj) {
                if(c.first == v.first) {
                    // constrained vertex found, find an uncolored edge
                    for(const auto& edge : v.second) {
                        if(edge.color == 0) {
                            startV1 = edge.v1;
                            startV2 = edge.v2;
                            startColor = edge.color;
                            goto done;
                        }
                    }
                }
            }
        }

        // else, look for some unconstrainted edge that is not colored
        for(const auto& v : adj) {
            for(const auto& edge : v.second) {
                if(edge.color == 0) {
                    startV1 = edge.v1;
                    startV2 = edge.v2;
                    startColor = edge.color;
                    goto done;
                }
            }
        }
done:

        setK.insert(startV1);
        setK.insert(startV2);
        colorEdge(startV1, startV2, legalColoringsOfEdge(startV1, startV2)[0]);
        numUncolored -= 1;

        bool edgeWasColored = true;
        while(edgeWasColored) {
            edgeWasColored = false;

            for(int v : setK) {
                for(auto& edge : adj[v]) {
                    if(edge.color != 0) {
                        continue;
                    }
                    std::vector<int> cols = legalColoringsOfEdge(edge.v1, edge.v2);
                    colorEdge(edge.v1, edge.v2, cols[0]);
                    setK.insert(edge.v1);
                    setK.insert(edge.v2);
                    numUncolored -= 1;
                    edgeWasColored = true;
                    numColoredInThisTree++;
                }
            }
        }
        std::cout << "COLORED A TREE WITH " << numColoredInThisTree << 
                     " EDGES" << std::endl;
    }

}

std::vector<int> Graph::legalColoringsOfEdge(const int v1, const int v2) const {
    const auto legalsOfV1 = legalColoringsOf(v1), legalsOfV2 = legalColoringsOf(v2);
    std::vector<int> legalsOfEdge;
    if(legalsOfV1.empty() && !legalsOfV2.empty()) {
        // empty means any color is fine
        legalsOfEdge = legalsOfV2;
    } else if(legalsOfV2.empty() && !legalsOfV1.empty()) {
        // empty means any color is fine
        legalsOfEdge = legalsOfV1;
    } else if(legalsOfV2.empty() && legalsOfV1.empty()) {
        // if everything is legal, start with color "1"
        legalsOfEdge.emplace_back(1);
    } else {
        for(const int a : legalsOfV1) {
            for(const int b : legalsOfV2) {
                if(a == b) {
                    legalsOfEdge.emplace_back(a);
                }
            }
        }
    }
    return legalsOfEdge;
}

void Graph::moveEdgeToAnotherGraph(Graph& other, const int v1, const int v2) {
    int i = 0, color;
    for(auto edge : adj.at(v1)) {
        if((edge.v1 == v1 && edge.v2 == v2) || (edge.v2 == v1 && edge.v1 == v2)) {
            color = edge.color;
            adj.at(v1).erase(adj.at(v1).begin() + i);
            if(adj.at(v1).size() == 0) {
                adj.erase(v1);
            }
            break;
        }
        i++;
    }
    i = 0;
    for(auto edge : adj.at(v2)) {
        if((edge.v1 == v1 && edge.v2 == v2) || (edge.v2 == v1 && edge.v1 == v2)) {
            adj.at(v2).erase(adj.at(v2).begin() + i);
            if(adj.at(v2).size() == 0) {
                adj.erase(v2);
            }
            break;
        }
        i++;
    }
    if(!other.isEdge(v1, v2)) {
        other.addEdge(Edge(v1, v2, color));
    }
}

void Graph::moveAllEdgesToAnotherGraph(Graph& other) {
    for(auto& v : adj) {
        for(auto& e : v.second) {
            if(!other.isEdge(e.v1, e.v2)) {
                other.addEdge(Edge(e.v1, e.v2, e.color));
            }
        }
    }
    adj.clear();
}

bool Graph::moveHangingEdgesTo(Graph& outGraph) {
    bool movedSomething = false;
    while(true) {
        Edge* e = findHangingEdge();
        if(!e) {
            break;
        }
        std::cout << "MOVING HANGING EDGE " << e->v1 << "," << e->v2 << std::endl;
        moveEdgeToAnotherGraph(outGraph, e->v1, e->v2);
        movedSomething = true;
    }
    return movedSomething;
}

Edge* Graph::findHangingEdge() {
    for(const auto& v : adj) {
        if(v.second.size() == 1) {
            return &getEdge(v.second[0].v1, v.second[0].v2);
        }
    }
    return nullptr;
}

void Graph::color(Graph& outGraph) {
    AdjList a;
    auto tempGraph = Graph(a);

    bool didSomething = true;
    while(didSomething) {
        std::cout << " ============= NEXT ITERATION" << std::endl;
        printGraphs(tempGraph, outGraph);

        didSomething = false;
        const bool moved = moveHangingEdgesTo(tempGraph);
        if(moved) {
            std::cout << "SOME EDGES WERE MOVED TO TEMPGRAPH" << std::endl;
            printGraphs(tempGraph, outGraph);
        }
        if(adj.size() == 0) {
            // there are no cycles and tempGraph contains a forest. Color the forest.
            std::cout << "NO CYCLES" << std::endl;
            if(!tempGraph.getAdj().empty()) {
                std::cout << "COLORING FOREST" << std::endl;
                tempGraph.colorAsForest();
                didSomething = true;
                std::cout << "COLORED TEMPGRAPH AS FOREST" << std::endl;
                printGraphs(tempGraph, outGraph);
                tempGraph.moveAllEdgesToAnotherGraph(outGraph);
            }
        } else {
            // there are cycles, find one
            std::cout << "FINDING CYCLE" << std::endl;
            const std::vector<int> verticesInCycle = findCycle();            
            auto edgesInCycle = pathEdges(verticesInCycle);
            // color it
            const bool success = colorPath(edgesInCycle.begin(), edgesInCycle.end());
            if(success) {

                for(size_t i = 0; i < verticesInCycle.size()-1; i++) {
                    const int v1 = verticesInCycle[i], v2 = verticesInCycle[i+1];
                    const int color = getEdge(v1, v2).color;

                    // export new constraints to temp graph
                    tempGraph.addVertexConstraint(v1, color);
                    tempGraph.addVertexConstraint(v2, color);
                    addVertexConstraint(v1, color);
                    addVertexConstraint(v2, color);

                    // delete this cycle from graph
                    moveEdgeToAnotherGraph(outGraph, v1, v2);

                }
                printGraphs(tempGraph, outGraph);
                didSomething = true;
            }
        }
    }
}

void Graph::print() const {
    if(adj.empty() && constraints.empty()) {
        std::cout << "~~EMPTY~~" << std::endl;
    } else {
        for(const auto& v : adj) {
            std::cout << v.first << ": ";
            for(const auto& e : v.second) {
                std::cout << e.v2 << "(" << e.color << "), ";
            }
            if(constraints.find(v.first) != constraints.end()) {
                std::cout << "constraints: [";
                for(const int c : constraints.at(v.first)) {
                    std::cout << c << ", ";
                }
                std::cout << "]";
            }
            std::cout << std::endl;
        }
    }
}

bool Graph::isOK(const int vertexIndex) {
    if (areGaps(vertexIndex)) {
        return false;
    }
    std::vector<int> seenColors;
    for(const auto& e : adj.at(vertexIndex)) {
        if(e.color == 0) {
            return false;
        }
        for(const int seen : seenColors) {
            if(seen == e.color) {
                return false;
            }
        }
        seenColors.emplace_back(e.color);
    }
    return true;
}

bool Graph::isEdge(const int v1, const int v2) {
    if(adj.find(v1) == adj.end()) {
        return false;
    }
    for(auto& edge : adj.at(v1)) {
        if((edge.v1 == v1 && edge.v2 == v2) || (edge.v2 == v1 && edge.v1 == v2)) {
            return true;
        }
    }
    return false;
}

void Graph::addVertexConstraint(const int vertexIndex, const int color) {
    constraints[vertexIndex].insert(color);
}

std::vector<int> Graph::getAllVertexConstraints(const int vertexIndex) const {
    std::vector<int> result;
    if(constraints.find(vertexIndex) != constraints.end()) {
        // there are artificial constraints
        for(const int c : constraints.at(vertexIndex)) {
            result.emplace_back(c);
        }
    }
    // normal edges
    for(const auto& e : adj.at(vertexIndex)) {
        if(e.color != 0) {
            result.emplace_back(e.color);
        }
    }
    return result;
}

void Graph::printGraphs(const Graph& temp, const Graph& out) const {
    std::cout << "  GRAPH: " << std::endl;
    print();
    std::cout << "  TEMPGRAPH: " << std::endl;
    temp.print();
    std::cout << "  OUTGRAPH: " << std::endl;
    out.print();
}