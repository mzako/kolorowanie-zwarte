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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}