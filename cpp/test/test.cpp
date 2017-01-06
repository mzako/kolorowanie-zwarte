#include <gtest/gtest.h>
#include <algorithm>

#include "../include/graph.h"

Graph generateSimpleLoopGraphWith10Vertices() {
    AdjList a;

    for(unsigned i = 1; i <= 10; i++) {
        a[i] = std::vector<Edge>();
    }

    for(unsigned i = 1; i < 10; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }
    a[10].emplace_back(10, 1);
    a[1].emplace_back(1, 10);

    return Graph(a);
}

Graph generateSimpleTreeGraph() {
    AdjList a;

    for(unsigned i = 1; i <= 10; i++) {
        a[i] = std::vector<Edge>();
    }

    for(unsigned i = 1; i < 3; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[1].emplace_back(1, 4);
    a[4].emplace_back(4, 1);

    for(unsigned i = 4; i < 6; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[5].emplace_back(5, 7);
    a[7].emplace_back(7, 5);

    for(unsigned i = 7; i < 10; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    return Graph(a);
}

Graph generateASimpleForestGraph() {
    AdjList a;

    for(unsigned i = 1; i <= 10; i++) {
        a[i] = std::vector<Edge>();
    }

    for(unsigned i = 1; i < 3; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[1].emplace_back(1, 4);
    a[4].emplace_back(4, 1);

    for(unsigned i = 4; i < 6; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    for(unsigned i = 7; i < 10; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    return Graph(a);
}

Graph generateEmptyGraph() {
    AdjList a;
    return Graph(a);
}


bool containersEqual(std::vector<int> a, std::vector<int> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

TEST(HelperFunctions, ContainerHelpersWork) {
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{1, 2, 3, 4, 5};
    EXPECT_TRUE(containersEqual(v1, v2));
    std::vector<int> v3{4, 5, 6};
    EXPECT_FALSE(containersEqual(v1, v3));
    std::vector<int> v4{8};
    std::vector<int> v5{5, 8};
    EXPECT_FALSE(containersEqual(v4, v5));
}

TEST(Backtracking, GettingEdgesPathFromIndicesWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();

    int i = 1;
    const auto edges = g.pathEdges(std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1});
    EXPECT_EQ(10, edges.size()); 
    for(unsigned i = 0; i < 10; i++) {
        const auto edge = edges[i];
        EXPECT_EQ(i+1, edge->v1);
        EXPECT_EQ(i+2, edge->v2);
        i++;
    }
    EXPECT_EQ(10, edges.back()->v1);
    EXPECT_EQ(1, edges.back()->v2);
}

TEST(Backtracking, AddingAnEdgeWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    for(unsigned i = 1; i <= 10; i++) {
        EXPECT_EQ(2, g.getAdj().at(i).size());
    }
    g.addEdge(Edge(3, 6));
    EXPECT_EQ(3, g.getAdj().at(3).size());
    EXPECT_EQ(3, g.getAdj().at(6).size());
}

TEST(Backtracking, DeterminingLegalColorOfAVertexWithoutAnyColorReturnsEmptyVector) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    const std::vector<int> actual = g.legalColoringsOf(1);
    EXPECT_TRUE(actual.empty());
}

TEST(Backtracking, DeterminingLegalColorOfAVertexWithEdgeColor1Returns23) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addEdge(Edge(1, 1000, 1));
    const std::vector<int> expectedColors{2, 3};
    const std::vector<int> actual = g.legalColoringsOf(1);
    EXPECT_TRUE(containersEqual(actual, expectedColors));
}

TEST(Backtracking, DeterminingLegalColoringsOfAVertexWithLowestColor1Works) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addEdge(Edge(1, 1000, 1));
    g.addEdge(Edge(1, 2000, 2));
    const std::vector<int> expectedColors{3, 4};
    const std::vector<int> actual = g.legalColoringsOf(1);
    EXPECT_TRUE(containersEqual(actual, expectedColors));
}

TEST(Backtracking, DeterminingLegalColoringsOfEdgeWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addEdge(Edge(1, 1000, 3));
    g.addEdge(Edge(1, 2000, 4));
    g.addEdge(Edge(2, 3000, 6));
    const std::vector<int> expectedColors{5};
    const std::vector<int> actual = g.legalColoringsOfEdge(1, 2);
    EXPECT_TRUE(containersEqual(actual, expectedColors));
}

TEST(Backtracking, FindingLowestAndHighestVertexColorWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.colorEdge(2, 3, 13);
    g.colorEdge(2, 1, 14);
    EXPECT_EQ(13, g.getLowestColor(2));
    EXPECT_EQ(14, g.getHighestColor(2));
}

TEST(Backtracking, FindingIfVertexHasColorGapsWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.colorEdge(2, 3, 13);
    g.colorEdge(2, 1, 14);
    EXPECT_FALSE(g.areGaps(2));
    g.colorEdge(2, 1, 15);
    EXPECT_TRUE(g.areGaps(2));
}

TEST(Backtracking, LegalColoringsOfAVertexWithAGapIsThatGap) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addEdge(Edge(4, 13, 0));
    g.colorEdge(4, 5, 1);
    g.colorEdge(4, 3, 3);
    /*     1     3
        3 --- 4 --- 5
              |
              |0
              |
              13
        Clearly, we must color edge 4-13 with color "2"
    */
    const std::vector<int> expectedColors{2};
    const std::vector<int> actual = g.legalColoringsOf(4);
    EXPECT_TRUE(containersEqual(actual, expectedColors));
}

TEST(Backtracking, VertexHasAsManyLegalColoringsAsCurrentMaxAndMinPlusMinusTwo) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addEdge(Edge(1, 1000, 6));
    g.addEdge(Edge(1, 2000, 7));
    const std::vector<int> expectedColors{5, 8, 4, 9};
    const std::vector<int> actual = g.legalColoringsOf(1);
    EXPECT_TRUE(containersEqual(actual, expectedColors));
}

TEST(Backtracking, ColoringAndGettingAnEdgeWorksBothWays) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.colorEdge(3, 4, 7);
    EXPECT_EQ(7, g.getEdge(3, 4).color);
    EXPECT_EQ(7, g.getEdge(4, 3).color);
}


TEST(Backtracking, ModifyingAColorIsSeenInAPath) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    const std::vector<int> indicesInPath{1, 2};
    auto edges = g.pathEdges(indicesInPath);
    g.colorEdge(1, 2, 7);
    EXPECT_EQ(7, edges[0]->color);
}

TEST(Backtracking, ZeroingColorsAlongPathWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.colorEdge(3, 4, 13);
    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);
    g.zeroPath(edges.begin(), edges.end());
    EXPECT_EQ(0, g.getEdge(3, 4).color);
}

TEST(Backtracking, LoopWithNoConstraintsColorsAlternate121212Etc) {
    auto g = generateSimpleLoopGraphWith10Vertices();

    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);

    const int maxColor = 10, startingIdx = indicesInPath[0];
    const bool success = g.colorPath(edges.begin(), edges.end());
    EXPECT_TRUE(success);

    const std::vector<int> expectedColors{1, 2, 1, 2, 1, 2, 1, 2, 1, 2};
    for(unsigned i = 1; i <= 9; i++) {
        EXPECT_EQ(expectedColors[i-1], g.getEdge(i, i+1).color)
            << "Current vertex: " << i;
    }
    EXPECT_EQ(expectedColors.back(), g.getEdge(10, 1).color)
            << "Current vertex: " << 10;
}

TEST(Backtracking, LoopWithContraint67Works) {
    auto g = generateSimpleLoopGraphWith10Vertices();

    // add a constraint for vertices 3 and 6 - they both have an edge of color 1.
    /*             _____________
          1    2  | 3    2      |
        1 -- 2 -- 3 -- 4 -- 5   |1
       2|                   |1  |
        | 1   2     3    2  |   |
        10-- 9 -- 8 -- 7 -- 6 --|
    */
    const int constraintColor = 1;
    g.addEdge(Edge(3, 7, constraintColor));

    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);

    const int maxColor = 10, startingIdx = indicesInPath[0];
    const bool success = g.colorPath(edges.begin(), edges.end());
    EXPECT_TRUE(success);
    // EXPECT_FALSE(success);

    const std::vector<int> expectedColors{1, 2, 3, 2, 1, 2, 3, 2, 1, 2};
    for(unsigned i = 1; i <= 9; i++) {
        EXPECT_EQ(expectedColors[i-1], g.getEdge(i, i+1).color)
            << "Current vertex: " << i;
    }
    EXPECT_EQ(expectedColors.back(), g.getEdge(10, 1).color)
            << "Current vertex: " << 10;
}

TEST(Backtracking, LoopWithContraint38Works) {
    auto g = generateSimpleLoopGraphWith10Vertices();

    // add a constraint for vertices 3 and 8 - they both have an edge of color 1.
    /* Possible coloring:
          1    2    3    2
        1 -- 2 -- 3 -- 4 -- 5
      2 |        1|         | 1
        |         |         |
        10-- 9 -- 8 -- 7 -- 6
          1    2    3    2
    */
    const int constraintColor = 1;
    g.addEdge(Edge(3, 8, constraintColor));

    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);

    const int maxColor = 10, startingIdx = indicesInPath[0];
    const bool success = g.colorPath(edges.begin(), edges.end());
    EXPECT_TRUE(success);

    const std::vector<int> expectedColors{1, 2, 3, 2, 1, 2, 3, 2, 1, 2};
    for(unsigned i = 1; i <= 9; i++) {
        EXPECT_EQ(expectedColors[i-1], g.getEdge(i, i+1).color)
            << "Current vertex: " << i;
    }
    EXPECT_EQ(expectedColors.back(), g.getEdge(10, 1).color)
            << "Current vertex: " << 10;
}

TEST(Backtracking, LoopTriangleDoesNotGetColoredAndReturnsFalse) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addEdge(Edge(3, 1, 0));
    const std::vector<int> indicesInPath{1, 2, 3, 1};
    auto edges = g.pathEdges(indicesInPath);

    const int maxColor = 10, startingIdx = indicesInPath[0];
    const bool success = g.colorPath(edges.begin(), edges.end());
    EXPECT_FALSE(success);

    for(unsigned i = 1; i <= 9; i++) {
        EXPECT_EQ(0, g.getEdge(i, i+1).color)
            << "Current vertex: " << i;
    }
    EXPECT_EQ(0, g.getEdge(10, 1).color)
            << "Current vertex: " << 10;
}

TEST(Cycle, FindingCycleInLoopGraphWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    const auto& cycle = g.findCycle();
    EXPECT_EQ(11, cycle.size());

    for(unsigned i = 0; i < 10; i++) {
        EXPECT_EQ(i+1, cycle[i]);
    }
    EXPECT_EQ(1, cycle[10]);
}

TEST(Cycle, FindingCycleInTreeReturnsEmpty) {
    auto g = generateSimpleTreeGraph();
    const auto& cycle = g.findCycle();
    EXPECT_EQ(0, cycle.size());
}

TEST(Forest, ColoringATreeWorks) {
    auto g = generateSimpleTreeGraph();
    g.colorAsForest();
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_FALSE(g.areGaps(v.first));
    }
}

TEST(Forest, ColoringAForestWorks) {
    auto g = generateASimpleForestGraph();
    g.colorAsForest();
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_FALSE(g.areGaps(v.first));
    }
}

TEST(Graph, MovingEdgeToOutputRemovesItFromGraph) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    int numEdgesBefore = 0;
    for(auto& v : g.getAdj()) {
        for(auto& e : v.second) {
            numEdgesBefore++;
        }
    }
    numEdgesBefore /= 2;
    g.moveEdgeToAnotherGraph(outG, 2, 3);
    int numEdgesAfter = 0;
    for(auto& v : g.getAdj()) {
        for(auto& e : v.second) {
            numEdgesAfter++;
        }
    }
    numEdgesAfter /= 2;
    EXPECT_EQ(1, numEdgesBefore - numEdgesAfter);
}

TEST(Graph, MovingAllEdgesOfVertexRemovesVertex) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    int numVerticesBefore = g.getAdj().size();
    g.moveEdgeToAnotherGraph(outG, 2, 3);
    g.moveEdgeToAnotherGraph(outG, 1, 2);
    int numVerticesAfter = g.getAdj().size();
    EXPECT_EQ(1, numVerticesBefore - numVerticesAfter);
}

TEST(Graph, MovingEdgeToAnotherGraphMakesItAppearInIt) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    EXPECT_EQ(0, outG.getAdj().size());
    g.moveEdgeToAnotherGraph(outG, 2, 3);
    EXPECT_EQ(2, outG.getAdj().size());
    const auto& v2 = outG.getAdj().at(2);
    const auto& v3 = outG.getAdj().at(3);
    EXPECT_EQ(1, v2.size());
    EXPECT_EQ(1, v2.size());
    EXPECT_EQ(2, v2[0].v1);
    EXPECT_EQ(3, v2[0].v2);
    EXPECT_EQ(3, v3[0].v1);
    EXPECT_EQ(2, v3[0].v2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}