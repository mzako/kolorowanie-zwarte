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
        for(size_t j = 0; j < adj[currentVertex].size(); j++) {
            if(adj[currentVertex][j].v2 == nextVertex) {
                edges.emplace_back(&adj[currentVertex][j]);
                break;
            }
        }
    }
    // loop around
    int lastVertex = elem.back(), firstVertex = elem[0];
    for(size_t j = 0; j < adj[lastVertex].size(); j++) {
        if(adj[lastVertex][j].v2 == firstVertex) {
            edges.emplace_back(&adj[lastVertex][j]);
            break;
        }
    }
    return edges;
}

bool Graph::colorPath(std::vector<Edge*>::iterator edge, std::vector<Edge*>::iterator end) {

    // looped around?
    if(edge == end) {
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
        std::vector<int> colors;
        for(const auto& edge : adj.at(vertexIndex)) {
            colors.emplace_back(edge.color);
        }
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
    for(const auto& edge : adj.at(vertexIndex)) {
        sum += edge.color;
    }
    const int expectedSum = (highestColor * (highestColor + 1) - 
        lowestColor * (lowestColor + 1) ) / 2;
    return sum < expectedSum;
}

int Graph::getLowestColor(const int vertexIndex) const {
    int lowestColor = -1;
    for(const auto& edge : adj.at(vertexIndex)) {
        if(edge.color > 0) {
            if(lowestColor == -1) {
                // first non-zero colour found
                lowestColor = edge.color;
            }
            if(edge.color < lowestColor) {
                lowestColor = edge.color;
            }
        }
    }
    return lowestColor;
}

int Graph::getHighestColor(const int vertexIndex) const {
    int highestColor = -1;
    for(const auto& edge : adj.at(vertexIndex)) {
        if(edge.color > 0) {
            if(highestColor == -1) {
                // first non-zero colour found
                highestColor = edge.color;
            }
            if(edge.color > highestColor) {
                highestColor = edge.color;
            }
        }
    }
    return highestColor;
}

std::vector<int> Graph::findCycle() {
    // cleanup labels
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

void Graph::colorAsTree() {
    int numUncolored = 0;
    for(auto& v : adj) {
        for(auto& e : v.second) {
            numUncolored++;
        }
    }
    numUncolored /= 2;

    std::cout << "COLORING TREE WITH " << numUncolored << " EDGES" << std::endl;

    std::set<int> setK;
    const auto& startingEdge = adj.begin()->second[0];

    setK.insert(startingEdge.v1);
    setK.insert(startingEdge.v2);
    colorEdge(startingEdge.v1, startingEdge.v2, 1);
    numUncolored -= 1;
    std::cout << "NUM UNCOLORED: " << numUncolored << std::endl;

    while(numUncolored != 0) {

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
                std::cout << "NUM UNCOLORED: " << numUncolored << std::endl;
            }
        }
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