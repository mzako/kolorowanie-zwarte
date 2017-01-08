#include "../include/graph.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <deque>

#define LOGLEVEL 3

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

    std::ofstream filetxt(fileName + ".txt");

    if(filetxt) {
        for (auto& kv : adj) {
            filetxt << kv.first << ": ";
            for (auto& edge : kv.second) {
                filetxt << edge.v2 << "(" << edge.color << "), "; 
            }
            filetxt << std::endl;
        }
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
    return edges;
}

bool Graph::colorPath(std::vector<Edge*> edges) {

    std::cout << " === COLORING PATH" << std::endl;

    int startingIndex = 0;
    for(size_t i = 0; i < edges.size(); i++) {
        // find a constrained vertex
        auto legals = legalColoringsOf(edges[i]->v1);
        if(!legals.empty()) {
            startingIndex = i;
            std::cout << "FOUND A CONSTRAINT AT ELEMENT " << startingIndex << 
                        " OF PATH" << std::endl;
            break;
        }
    }

    // loop around path so that we start with constrainted vertex
    std::vector<Edge*> offsetEdges;
    const int numEdges = edges.size();
    for(int i = 0; i < numEdges; i++) {
        offsetEdges.emplace_back(edges[(i+startingIndex) % numEdges]);
    }
    //offsetEdges.emplace_back(edges.front());

    std::cout << "APPLIED OFFSET: ";
    for(const auto e : offsetEdges) {
        std::cout << e->v1 << ", ";
    }
    std::cout << std::endl;

    return colorPathRecur(offsetEdges.begin(), offsetEdges.end());
}

bool Graph::colorPathRecur(std::vector<Edge*>::iterator edge, 
    std::vector<Edge*>::iterator end) {

    // looped around?
    if(edge == end) {
        std::cout << "BACKTRACK COLORING REACHED END" << std::endl;
        return true;
    }

    const int currentVertexIdx = (*edge)->v1, nextVertexIdx = (*edge)->v2;

    const std::vector<int> legalsOfEdge = legalColoringsOfEdge(currentVertexIdx, 
        nextVertexIdx);

    // int lowestLegal, highestLegal;
    // if(!legalsOfEdge.empty()) {
    //     auto legalsSorted = legalsOfEdge;
    //     std::sort(legalsSorted.begin(), legalsSorted.end());
    //     if(legalsSorted.size() == 4) {
    //         lowestLegal = legalsSorted.front();
    //     }
    //     if(legalsSorted.size() > 3) {
    //         highestLegal = legalsSorted.back();
    //     }
    // }

    // std::cout << "Possible colorings of " << currentVertexIdx << " " << nextVertexIdx
    //             << std::endl;
    // for(const int c : legalsOfEdge) {
    //     std::cout << c << ",";
    // }
    // std::cout << std::endl; 
    for(const int currentColor : legalsOfEdge) {


        // don't do the 'gap' skip if we're on the last element
        // if(((edge+1) == end || (edge+2) == end) && 
        //     (currentColor == lowestLegal || currentColor == highestLegal)) {
        //     std::cout << "WON'T TRY GAP SKIP BECAUSE WE'RE THE LAST ELEMENT" << std::endl;
        //     continue;
        // }

        std::cout << "TRYING COL: " << currentColor << std::endl;
        colorEdge(currentVertexIdx, nextVertexIdx, currentColor);
        if(colorPathRecur(++edge, end)) {
            // see if our colors are fine
            if(!areGaps(currentVertexIdx)) {
                return true;
            } else {
                std::cout << "COLOR GAPS FOUND AT VERTEX " << currentVertexIdx << 
                    " ZEROING PATH AHEAD." << std::endl;
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

        std::cout << "CYCLE FOUND: ";
        for(const auto& el : result) {
            std::cout << el << ", ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "NO CYCLE FOUND" << std::endl;
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

bool Graph::colorAsForest() {
    int numUncolored = numEdges();
    std::cout << " === COLORING A FOREST WITH " << numUncolored << " EDGES" << std::endl;

    AdjList a;
    auto tempGraph = Graph(a);
    auto outGraph = Graph(a);
    moveAllEdgesToAnotherGraph(tempGraph);

    std::deque<Graph*> graphQueue;

    bool justAddedToQueue = false;
    int numTries = 0;
    const int maxNumTries = 10;
    while(numUncolored != 0) {
        std::cout << " = NEXT FOREST COLORING ITERATION" << std::endl;
        if(LOGLEVEL > 2) {
            std::cout << "TEMP FOREST GRAPH: " << std::endl;
            tempGraph.print();
        }

        if(numTries > maxNumTries) {
            std::cout << "TRIED TO COLOR THE FOREST " << numTries << " TIMES, FAILED."
                      << std::endl;
            break;
        }
        numTries++; 
        std::cout << "FINDING PATH IN FOREST" << std::endl;

        const std::vector<int> verticesInPath = tempGraph.findPath();
        if(verticesInPath.empty()) {
            std::cout << "TREE APPEARS TO BE EMPTY" << std::endl;
            if(graphQueue.empty()) {
                std::cout << "QUEUE IS ALSO EMPTY, DONE" << std::endl;
                break;
            } else {
                std::cout << "MOVING FROM QUEUE" << std::endl;
                graphQueue.front()->moveAllEdgesToAnotherGraph(tempGraph);
                graphQueue.pop_front();
                continue;
            }
        }

        auto edges = tempGraph.pathEdges(verticesInPath);
        const bool success = tempGraph.colorPath(edges);
        

        if(success) {
            std::cout << "COLORING WAS SUCCESSFUL" << std::endl;
            
            for(size_t i = 0; i < verticesInPath.size()-1; i++) {
                const int v1 = verticesInPath[i], v2 = verticesInPath[i+1];
                const int color = tempGraph.getEdge(v1, v2).color;
                
                for(auto& g : graphQueue) {
                    g->addVertexConstraint(v1, color);
                    g->addVertexConstraint(v2, color);
                }
                tempGraph.addVertexConstraint(v1, color);
                tempGraph.addVertexConstraint(v2, color);
                tempGraph.moveEdgeToAnotherGraph(outGraph, v1, v2);
            }
        } else {
            std::cout << "FAILED TO COLOR, WILL MOVE TO QUEUE" << std::endl;
            AdjList aa;
            Graph* newGraph = new Graph(aa);
            for(size_t i = 0; i < verticesInPath.size()-1; i++) {
                const int v1 = verticesInPath[i], v2 = verticesInPath[i+1];
                tempGraph.moveEdgeToAnotherGraph(*newGraph, v1, v2);
            }
            graphQueue.push_back(newGraph);
            justAddedToQueue = true;
        }
    }

    for(auto& g : graphQueue) {
        g->moveAllEdgesToAnotherGraph(*this);
    }
    graphQueue.clear();

    tempGraph.moveAllEdgesToAnotherGraph(*this);
    outGraph.moveAllEdgesToAnotherGraph(*this);
    if(tempGraph.getAdj().empty() && graphQueue.empty()) {
        return true;
    }
    return false;
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
        legalsOfEdge.emplace_back(10);
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
        if(constraints.find(v1) != constraints.end()) {
            for(const int c : constraints.at(v1)) {
                other.addVertexConstraint(v1, c);
            }
        }
        if(constraints.find(v2) != constraints.end()) {
            for(const int c : constraints.at(v2)) {
                other.addVertexConstraint(v2, c);
            }
        }
    }
}

void Graph::moveAllEdgesToAnotherGraph(Graph& other) {
    for(auto& v : adj) {
        for(auto& e : v.second) {
            if(!other.isEdge(e.v1, e.v2)) {
                other.addEdge(Edge(e.v1, e.v2, e.color));
            }
        }
        if(constraints.find(v.first) != constraints.end()) {
            for(const int c : constraints.at(v.first)) {
                other.addVertexConstraint(v.first, c);
            }
        }
    }
    adj.clear();
    constraints.clear();
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

bool Graph::color(Graph& outGraph) {
    AdjList a;
    auto tempGraph = Graph(a);

    std::deque<Graph*> graphQueue;

    bool justAddedToQueue = true;

    bool didSomething = true;
    const int triesThreshold = 3;
    int triesDidNothing = 0;
    while(true) {
        if(!didSomething) {
            triesDidNothing++;
        } else {
            triesDidNothing = 0;
        }
        if(triesDidNothing >= triesThreshold) {
            std::cout << "TRIED " << triesDidNothing << " TIMES BUT DID NOTHING" << std::endl;
            break;
        }

        std::cout << " ============= NEXT ITERATION" << std::endl;

        if(!graphQueue.empty()) {
            if(justAddedToQueue) {
                std::cout << "QUEUE NOT EMPTY BUT JUST ADDED SOMETHING" << std::endl;
            } else {
                std::cout << "ADDING FROM QUEUE" << std::endl;
                Graph* popped = graphQueue.front();
                popped->moveAllEdgesToAnotherGraph(*this);
                graphQueue.pop_front();
           }
        }
        justAddedToQueue = false;

        printGraphs(tempGraph, outGraph);

        didSomething = false;
        const bool moved = moveHangingEdgesTo(tempGraph);
        if(moved) {
            std::cout << "SOME EDGES WERE MOVED TO TEMPGRAPH" << std::endl;
            printGraphs(tempGraph, outGraph);
        }
        if(adj.empty()) {
            // there are no cycles and tempGraph contains a forest.
            std::cout << "NO CYCLES" << std::endl;


            if(!tempGraph.getAdj().empty()) {
                // std::cout << "COLORING FOREST" << std::endl;
                // std::vector<int> verticesInPath = tempGraph.findPath();

                // std::cout << "DUPA1" << std::endl;

                // AdjList a;
                // //auto* newGraph = new Graph(a);
                // std::cout << "DUPA2" << std::endl;
                
                // for(size_t i = 0; i < verticesInPath.size()-1; i++) {
                // std::cout << "DUPA4" << std::endl;
                    
                //     const int v1 = verticesInPath[i], v2 = verticesInPath[i+1];
                // std::cout << "DUPA5" << std::endl;
                    
                //     tempGraph.moveEdgeToAnotherGraph(*this, v1, v2);
                // }
                // std::cout << "DUPA3" << std::endl;
                
                //graphQueue.push_back(newGraph);
                //justAddedToQueue = true;
                //didSomething = true;

                const bool success = tempGraph.colorAsForest();
                if(!success) {
                    didSomething = false;
                } else {
                    didSomething = true;
                    std::cout << "COLORED TEMPGRAPH AS FOREST" << std::endl;
                    printGraphs(tempGraph, outGraph);
                }

                // // move all new constraints to graph and everything in the queue
                for(const auto& v : tempGraph.getAdj()) {
                    for(const auto& e : v.second) {
                        addVertexConstraint(v.first, e.color);
                        for(auto& g : graphQueue) {
                            g->addVertexConstraint(v.first, e.color);
                        }
                    }
                }

                tempGraph.moveAllEdgesToAnotherGraph(outGraph);
            }
        } else {
            // there are cycles, find one
            std::cout << "FINDING CYCLE" << std::endl;
            const std::vector<int> verticesInCycle = findCycle();

            const std::vector<int> constraintsInCycle = 
                findConstrainedVerticesInCycle(verticesInCycle);

            std::cout << "FOUND A CYCLE WITH " << constraintsInCycle.size()
                        << " CONSTRAINTS: ";
            for(const auto c : constraintsInCycle) {
                std::cout << c << ", ";
            }
            std::cout << std::endl;

            if(constraintsInCycle.size() <= 1) {
                // there's at most one constraint in the cycle

                auto edgesInCycle = pathEdges(verticesInCycle);
                // color it
                const bool success = colorPath(edgesInCycle);
                if(success) {
                    std::cout << "COLORING PATH SUCCESSFUL" << std::endl;

                    for(size_t i = 0; i < verticesInCycle.size()-1; i++) {
                        const int v1 = verticesInCycle[i], v2 = verticesInCycle[i+1];
                        const int color = getEdge(v1, v2).color;

                        // export new constraints to temp graph, original graph and
                        // all graphs in queue
                        tempGraph.addVertexConstraint(v1, color);
                        tempGraph.addVertexConstraint(v2, color);
                        addVertexConstraint(v1, color);
                        addVertexConstraint(v2, color);
                        for(auto& g : graphQueue) {
                            g->addVertexConstraint(v1, color);
                        }

                        // delete this cycle from graph
                        moveEdgeToAnotherGraph(outGraph, v1, v2);
                    }
                    printGraphs(tempGraph, outGraph);
                    didSomething = true;
                } else {
                    // failed to color it, move it to queue
                    std::cout << "FAILED TO COLOR, MOVING TO QUEUE" << std::endl;
                    AdjList a;
                    auto* newGraph = new Graph(a);
                    for(size_t i = 0; i < verticesInCycle.size()-1; i++) {
                        const int v1 = verticesInCycle[i], v2 = verticesInCycle[i+1];
                        moveEdgeToAnotherGraph(*newGraph, v1, v2);
                    }
                    graphQueue.push_back(newGraph);
                    justAddedToQueue = true;
                    didSomething = true;
                }
            } else {
                // there are two or more constraints in the cycle

                const auto paths = splitCycle(verticesInCycle, constraintsInCycle);
            
                std::cout << "SPLIT CYCLE INTO " << paths.size() << " PATHS" << std::endl;

                AdjList a;
                std::vector<Graph> pathGraphs(paths.size(), Graph(a));

                // move each path to a own graph
                for(size_t i = 0; i < paths.size(); i++) {
                    const auto currentPath = paths[i];

                    for(size_t j = 0; j < currentPath.size()-1; j++) {
                        const int v1 = currentPath[j], v2 = currentPath[j+1];
                        // /const int color = getEdge(v1, v2).color;
                        moveEdgeToAnotherGraph(pathGraphs[i], v1, v2);
                    }
                }

                // color each path alone
                for(size_t i = 0; i < paths.size(); i++) {
                    pathGraphs[i].print();

                    const auto currentPath = paths[i];

                    std::cout << "CURRENT PATH: " << i << ": ";
                    for(const int v : currentPath) {
                        std::cout << v << ", ";
                    }
                    std::cout << std::endl;

                    auto edges = pathGraphs[i].pathEdges(paths[i]);
                    const bool success = pathGraphs[i].colorPath(edges);

                    if(success) {
                        std::cout << "COLORING PATH SUCCESSFUL" << std::endl;
                        // export new constraints to tempgraph, original graph and
                        // to all next path graphs
                        for(size_t j = 0; j < currentPath.size()-1; j++) {
                            const int v1 = currentPath[j], v2 = currentPath[j+1];
                            const int color = pathGraphs[i].getEdge(v1, v2).color;
                            tempGraph.addVertexConstraint(v1, color);
                            tempGraph.addVertexConstraint(v2, color);
                            addVertexConstraint(v1, color);
                            addVertexConstraint(v2, color);
                            for(size_t w = i+1; w < pathGraphs.size(); w++) {
                                pathGraphs[w].addVertexConstraint(v1, color);
                                pathGraphs[w].addVertexConstraint(v2, color);
                            }
                            // delete this edge from graph
                            std::cout << "PATHGRAPH: " << std::endl;
                            pathGraphs[i].print();
                            pathGraphs[i].moveEdgeToAnotherGraph(outGraph, v1, v2);
                        }
                    } else {
                        std::cout << "FAILED TO COLOR, MOVING TO QUEUE" << std::endl;
                        AdjList a;
                        auto* newGraph = new Graph(a);
                        for(size_t j = 0; j < currentPath.size()-1; j++) {
                            const int v1 = currentPath[j], v2 = currentPath[j+1];
                            pathGraphs[i].moveEdgeToAnotherGraph(*newGraph, v1, v2);
                        }
                        graphQueue.push_back(newGraph);
                        justAddedToQueue = true;
                        didSomething = true;
                    }
                }
            } // else
        }
    }
    graphQueue.clear();
    if(adj.empty() && tempGraph.getAdj().empty()) {

        for(const auto& v : outGraph.getAdj()) {
            for(const auto& e : v.second) {
                if(e.color == 0) {
                    return false;
                }
            }
            if(!outGraph.isOK(v.first)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Graph::print() const {
    if(LOGLEVEL < 3) {
        return;
    }

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
    for(const int c : getAllVertexConstraints(vertexIndex)) {
        if(c == 0) {
            return false;
        }
        for(const int seen : seenColors) {
            if(seen == c) {
                return false;
            }
        }
        seenColors.emplace_back(c);
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
    std::set<int> result;
    if(constraints.find(vertexIndex) != constraints.end()) {
        // there are artificial constraints
        for(const int c : constraints.at(vertexIndex)) {
            result.insert(c);
        }
    }
    // normal edges
    for(const auto& e : adj.at(vertexIndex)) {
        if(e.color != 0) {
            result.insert(e.color);
        }
    }
    return std::vector<int>(result.begin(), result.end());
}

void Graph::printGraphs(const Graph& temp, const Graph& out) const {
    std::cout << "  GRAPH: " << std::endl;
    print();
    std::cout << "  TEMPGRAPH: " << std::endl;
    temp.print();
    std::cout << "  OUTGRAPH: " << std::endl;
    out.print();
}

std::vector<int> Graph::findConstrainedVerticesInCycle(std::vector<int> indices) const {
    indices.pop_back();
    std::vector<int> result;
    for(const int i : indices) {
        std::vector<int> c = getAllVertexConstraints(i);
        if(!c.empty()) {
            result.emplace_back(i);
        }
    }
    return result;
}

std::vector<std::vector<int> > Graph::splitCycle(std::vector<int> cycle, 
    std::vector<int> cutVertices) const {

    // remove last element from cycle
    cycle.pop_back();

    std::vector<std::vector<int> > result{};

    // start at a cut vertex
    const int startingVertex = cutVertices[0];
    int startingVertexIdx = 0;
    for(const int v : cycle) {
        if(v == startingVertex) {
            break;
        }
        startingVertexIdx++;
    }

    int pathIdx = -1;
    for(size_t i = 0; i < cycle.size()+1; i++) {
        
        const int currVertexIdx = cycle[(i + startingVertexIdx) % cycle.size()];

        // are we in a cut vertex?
        for(const int c : cutVertices) {
            if(currVertexIdx == c) {
                if(pathIdx >= 0) {
                    result[pathIdx].emplace_back(currVertexIdx);
                }
                
                pathIdx++;
                result.emplace_back(std::vector<int>{});
                break;
            }
        }
        result[pathIdx].emplace_back(currVertexIdx);
    }
    // remove last path as it has just 1 element
    result.pop_back();

    return result;
}

int Graph::numEdges() const {
    int n = 0;
    for(auto& v : adj) {
        for(auto& e : v.second) {
            n++;
        }
    }
    n /= 2; // each edge is counted twice
    return n;
}

std::vector<int> Graph::findPath() {
    // cleanup labels
    labels.clear();
    for(auto& keyval : adj) {
        labels[keyval.first] = false;
    }

    if(adj.empty()) {
        return {};
    }

    const int startingVertexIdx = adj.begin()->first;
    auto result = findPathRecur(startingVertexIdx, startingVertexIdx);
    if(!result.empty()) {
        std::reverse(result.begin(), result.end());
    }

    std::cout<< "FOUND PATH: ";
    for(const int a : result) {
        std::cout << a << ", ";
    }
    std::cout << std::endl;

    // cleanup labels
    for(auto& keyval : labels) {
        keyval.second = false;
    }
    return result;
}

std::vector<int> Graph::findPathRecur(const int startingVertexIdx, 
    const int currentVertexIdx) {
    labels[currentVertexIdx] = true;
    const auto& edges = adj.at(currentVertexIdx);
    if(startingVertexIdx != currentVertexIdx && edges.size() == 1) {
        // leaf found
        return {currentVertexIdx};
    }

    for(auto& edge : edges) {
        
        const int neighbourIdx = edge.v2;
        if(labels.at(neighbourIdx)) {
            continue;
        }

        auto result = findPathRecur(startingVertexIdx, neighbourIdx);
        if(!result.empty()) {
            result.emplace_back(currentVertexIdx);
            // return the first branch since we're looking just for paths
            return result;
        }
    }

}