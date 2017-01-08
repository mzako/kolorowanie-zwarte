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

Graph generateSimpleForestGraph() {
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


Graph generateGraphWithOneLoopAndSomeHangingEges() {
    AdjList a;

    /* Loop 1-6
       Hanging 7-11 coming out from vertex 3
       Hanging 12-15 coming out from vertex 4
    */

    for(unsigned i = 1; i < 15; i++) {
        a[i] = std::vector<Edge>();
    }

    // loop
    for(unsigned i = 1; i < 6; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }
    a[6].emplace_back(6, 1);
    a[1].emplace_back(1, 6);

    // hanging 1
    for(unsigned i = 7; i < 11; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[3].emplace_back(3, 7);
    a[7].emplace_back(7, 3);

    // hanging 2
    for(unsigned i = 12; i < 15; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[4].emplace_back(4, 12);
    a[12].emplace_back(12, 4);

    return Graph(a);
}

Graph generateGraphTwoLoopsAndHangingEdges() {
    AdjList a;

    /* Loop 1-6
       Hanging 7-11 coming out from vertex 3
       Hanging 12-15 coming out from vertex 4
       Loop 16-20
    */

    for(unsigned i = 1; i < 20; i++) {
        a[i] = std::vector<Edge>();
    }

    // loop 1
    for(unsigned i = 1; i < 6; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }
    a[6].emplace_back(6, 1);
    a[1].emplace_back(1, 6);

    // hanging 1
    for(unsigned i = 7; i < 11; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[3].emplace_back(3, 7);
    a[7].emplace_back(7, 3);

    // hanging 2
    for(unsigned i = 12; i < 15; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[4].emplace_back(4, 12);
    a[12].emplace_back(12, 4);

    // loop 2
    for(unsigned i = 16; i < 19; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }
    a[19].emplace_back(19, 16);
    a[16].emplace_back(16, 19);

    return Graph(a);
}

Graph generateGraphTwoLoopsHangingEdgesAndTwoConnections() {
    AdjList a;

    /* Loop 1-6
       Hanging 7-11 coming out from vertex 3
       Hanging 12-15 coming out from vertex 4
       Loop 16-20
       Connection 15-16 (bridge between loops)
       Connection 18-2 (second bridge between loops)
    */

    for(unsigned i = 1; i < 20; i++) {
        a[i] = std::vector<Edge>();
    }

    // loop 1
    for(unsigned i = 1; i < 6; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }
    a[6].emplace_back(6, 1);
    a[1].emplace_back(1, 6);

    // hanging 1
    for(unsigned i = 7; i < 11; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[3].emplace_back(3, 7);
    a[7].emplace_back(7, 3);

    // hanging 2
    for(unsigned i = 12; i < 15; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }

    a[4].emplace_back(4, 12);
    a[12].emplace_back(12, 4);

    // loop 2
    for(unsigned i = 16; i < 19; i++) {
        a[i].emplace_back(i, i+1);
        a[i+1].emplace_back(i+1, i);
    }
    a[19].emplace_back(19, 16);
    a[16].emplace_back(16, 19);

    // 1st connection (bridge) 
    a[16].emplace_back(16, 15);
    a[15].emplace_back(15, 16);

    // 2nd connection (bridge) 
    a[18].emplace_back(18, 2);
    a[2].emplace_back(2, 18);

    return Graph(a);
}

Graph generateGraphK4() {
    AdjList a;

    for(unsigned i = 1; i <= 4; i++) {
        a[i] = std::vector<Edge>();
    }

    a[1].emplace_back(1, 2);
    a[1].emplace_back(1, 3);
    a[1].emplace_back(1, 4);

    a[2].emplace_back(2, 1);
    a[2].emplace_back(2, 3);
    a[2].emplace_back(2, 4);

    a[3].emplace_back(3, 1);
    a[3].emplace_back(3, 2);
    a[3].emplace_back(3, 4);

    a[4].emplace_back(4, 1);
    a[4].emplace_back(4, 2);
    a[4].emplace_back(4, 3);

    return Graph(a);
}

Graph generateTriangleGraph() {
    AdjList a;

    for(unsigned i = 1; i <= 3; i++) {
        a[i] = std::vector<Edge>();
    }

    a[1].emplace_back(1, 2);
    a[2].emplace_back(2, 1);

    a[3].emplace_back(3, 2);
    a[2].emplace_back(2, 3);

    a[3].emplace_back(3, 1);
    a[1].emplace_back(1, 3);

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

TEST(Backtracking, GettingAllConstraintsOfVertexReturnsItsEdgeColorsAndConstraints) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addVertexConstraint(3, 10);
    g.colorEdge(3, 4, 11);
    const std::vector<int> expectedConstraints{10, 11};
    const std::vector<int> actual = g.getAllVertexConstraints(3);
    EXPECT_TRUE(containersEqual(actual, expectedConstraints));
}

TEST(Backtracking, DeterminingLegalColoringsOfVertexTakesConstraintsIntoAccount) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addVertexConstraint(3, 10);
    const std::vector<int> expectedColors{8, 9, 11, 12};
    const std::vector<int> actual = g.legalColoringsOf(3);
    for(const int a : actual ) {
        std::cout << a << ", ";
    }
    std::cout << std::endl;
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

TEST(Backtracking, LoopWithNoConstraintsWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();

    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);

    const bool success = g.colorPath(edges);
    EXPECT_TRUE(success);

    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_FALSE(g.areGaps(v.first));
        EXPECT_TRUE(g.isOK(v.first));
    }
}

TEST(Backtracking, LoopWithContraint36Works) {
    auto g = generateSimpleLoopGraphWith10Vertices();

    // add a constraint for vertices 3 and 6 - they both have an edge of color 1.
    /*             _____________
          2    3  | 2    1      |
        1 -- 2 -- 3 -- 4 -- 5   |1
       1|                   |2  |
        | 2   1     2    3  |   |
        10-- 9 -- 8 -- 7 -- 6 --|
    */
    const int constraintColor = 1;
    g.addEdge(Edge(3, 7, constraintColor));

    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);

    const bool success = g.colorPath(edges);
    EXPECT_TRUE(success);

    const std::vector<int> expectedColors{2, 3, 2, 1, 2, 3, 2, 1, 2, 1};
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
          2    3    2    1
        1 -- 2 -- 3 -- 4 -- 5
      1 |        1|         | 2
        |         |         |
        10-- 9 -- 8 -- 7 -- 6
          2    3    2    1
    */
    const int constraintColor = 1;
    g.addEdge(Edge(3, 8, constraintColor));

    const std::vector<int> indicesInPath{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    auto edges = g.pathEdges(indicesInPath);

    const bool success = g.colorPath(edges);
    EXPECT_TRUE(success);

    const std::vector<int> expectedColors{2, 3, 2, 1, 2, 1, 2, 3, 2, 1};
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

    const bool success = g.colorPath(edges);
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

TEST(Forest, ColoringATreeWithSingleEdgeWorks) {
    auto g = generateEmptyGraph();
    g.addEdge(Edge(1, 2, 0));
    g.colorAsForest();
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_FALSE(g.areGaps(v.first));
        EXPECT_TRUE(g.isOK(v.first));
    }
}

TEST(Forest, ColoringATreeWorks) {
    auto g = generateSimpleTreeGraph();
    g.colorAsForest();
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_FALSE(g.areGaps(v.first));
        EXPECT_TRUE(g.isOK(v.first));
    }
}

TEST(Forest, ColoringATreeWithOneEdgeColorConstraintWorksWorks) {
    auto g = generateSimpleTreeGraph();
    g.addVertexConstraint(2, 20);
    g.colorAsForest();
    g.print();
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
            // probably they will all be greater than 10, since we constrain one
            // edge to color 20.
            EXPECT_LT(10, e.color); 
        }
        EXPECT_FALSE(g.areGaps(v.first));
        EXPECT_TRUE(g.isOK(v.first));
    }
}

TEST(Forest, ColoringAForestWorks) {
    auto g = generateSimpleForestGraph();
    g.colorAsForest();
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_FALSE(g.areGaps(v.first));
        EXPECT_TRUE(g.isOK(v.first));        
    }
}

TEST(Moving, MovingEdgeToOutputRemovesItFromGraph) {
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

TEST(Moving, MovingAllEdgesOfVertexRemovesVertex) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    int numVerticesBefore = g.getAdj().size();
    g.moveEdgeToAnotherGraph(outG, 2, 3);
    g.moveEdgeToAnotherGraph(outG, 1, 2);
    int numVerticesAfter = g.getAdj().size();
    EXPECT_EQ(1, numVerticesBefore - numVerticesAfter);
}

TEST(Moving, MovingEdgeToAnotherGraphMakesItAppearInIt) {
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

TEST(Moving, MovingMultipleTimesToAnotherGraphDoesNotRemoveStuffFromOtherGraph) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    g.moveEdgeToAnotherGraph(outG, 2, 3);
    EXPECT_EQ(2, outG.getAdj().size());
    g.moveEdgeToAnotherGraph(outG, 3, 4);
    EXPECT_EQ(3, outG.getAdj().size());
    g.moveEdgeToAnotherGraph(outG, 7, 8);
    EXPECT_EQ(5, outG.getAdj().size());
}

TEST(Moving, MovingAllEdgesFromGraphToGraphWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    g.moveAllEdgesToAnotherGraph(outG);
    EXPECT_EQ(0, g.getAdj().size());
    EXPECT_EQ(10, outG.getAdj().size());
}

TEST(Moving, MovingEdgesFromEmptyGraphToSomeGraphDoesNotRemoveStuff) {
    auto g = generateEmptyGraph();
    auto outG = generateSimpleLoopGraphWith10Vertices();
    g.moveAllEdgesToAnotherGraph(outG);
    EXPECT_EQ(0, g.getAdj().size());
    EXPECT_EQ(10, outG.getAdj().size());
}

TEST(Hanging, RecursivelyMovingHangingEdgesFromTreeToAnotherGraphMovesAllEdges) {
    auto g = generateSimpleTreeGraph();
    int sizeBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    g.moveHangingEdgesTo(outG);
    EXPECT_EQ(0, g.getAdj().size());
    EXPECT_EQ(sizeBefore, outG.getAdj().size());
}

TEST(Hanging, RecursivelyMovingHangingEdgesFromForestToAnotherGraphMovesAllEdges) {
    auto g = generateSimpleForestGraph();
    int sizeBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    g.moveHangingEdgesTo(outG);
    EXPECT_EQ(0, g.getAdj().size());
    EXPECT_EQ(sizeBefore, outG.getAdj().size());
}

TEST(Hanging, RecursivelyMovingHangingEdgesFromAGraphWithALoopDoesNotMoveTheLoop) {
    auto g = generateGraphWithOneLoopAndSomeHangingEges();
    // 15 vertices in graph, loop has 6 vertices
    int sizeBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    g.moveHangingEdgesTo(outG);

    int numEdgesInOriginalGraph = 0;
    for(const auto& v : g.getAdj()) {
        for(const auto& e : v.second) {
            numEdgesInOriginalGraph++;
        } 
    }
    EXPECT_EQ(6, numEdgesInOriginalGraph/2); // each edge is counted twice

    int numEdgesInOutputGraph = 0;
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            numEdgesInOutputGraph++;
        } 
    }
    EXPECT_EQ(9, numEdgesInOutputGraph/2);
}

TEST(Hanging, MovingAHangingEdgeReturnsTrue) {
    auto g = generateGraphWithOneLoopAndSomeHangingEges();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.moveHangingEdgesTo(outG));
}

TEST(Hanging, FailureToMoveAHangingEdgeReturnsFalse) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    auto outG = generateEmptyGraph();
    EXPECT_FALSE(g.moveHangingEdgesTo(outG));
}

TEST(Coloring, ColoringATreeWorks) {
    auto g = generateSimpleTreeGraph();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringAForestWorks) {
    auto g = generateSimpleForestGraph();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringALoopWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringAGraphWithALoopAndHangingEdgesWorks) {
    auto g = generateGraphWithOneLoopAndSomeHangingEges();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringAGraphWithTwoLoopsAndHangingEdgesWorks) {
    auto g = generateGraphTwoLoopsAndHangingEdges();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, DISABLED_ColoringAGraphWithTwoLoopsHangingEdgesAndTwoConnections) {
    auto g = generateGraphTwoLoopsHangingEdgesAndTwoConnections();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringACompleteGraphK4Works) {
    auto g = generateGraphK4();
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringATriangleWithTwoConstraintsWorks) {
    auto g = generateTriangleGraph();
    g.addVertexConstraint(1, 10);
    g.addVertexConstraint(2, 9);
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, ColoringATriangleWithThreeConstraintsWorks) {
    auto g = generateTriangleGraph();
    g.addVertexConstraint(1, 10);
    g.addVertexConstraint(2, 11);
    g.addVertexConstraint(3, 12);
    const int edgesBefore = g.numEdges(), verticesBefore = g.getAdj().size();
    auto outG = generateEmptyGraph();
    EXPECT_TRUE(g.color(outG));
    EXPECT_TRUE(!outG.getAdj().empty());
    for(const auto& v : outG.getAdj()) {
        for(const auto& e : v.second) {
            EXPECT_NE(0, e.color);
        }
        EXPECT_TRUE(outG.isOK(v.first));
    }
    EXPECT_EQ(edgesBefore, outG.numEdges());
    EXPECT_EQ(verticesBefore, outG.getAdj().size());
}

TEST(Coloring, DeterminingIfVertexIsColoredOKWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.colorEdge(2, 3, 1);
    g.colorEdge(3, 4, 2);
    EXPECT_TRUE(g.isOK(3));
    EXPECT_TRUE(g.isOK(4));
    g.colorEdge(6, 7, 1);
    g.colorEdge(7, 8, 3);
    EXPECT_FALSE(g.isOK(7));
}

TEST(Coloring, DeterminingIfEdgeExistsWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    EXPECT_TRUE(g.isEdge(3, 4));
    EXPECT_FALSE(g.isEdge(3, 5));
}

TEST(Cycle, FindingConstraintsOfCycleWithASingleConstraintReturnsThatConstraint) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addVertexConstraint(3, 100);
    const std::vector<int> indicesInCycle{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    std::vector<int> c = g.findConstrainedVerticesInCycle(indicesInCycle);
    EXPECT_EQ(1, c.size());
    EXPECT_EQ(3, c[0]);
}

TEST(Cycle, FindingConstraintsOfCycleWithAManyConstraintsReturnsThemAll) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    g.addVertexConstraint(3, 100);
    g.addVertexConstraint(3, 200);
    g.addVertexConstraint(4, 200);
    g.addVertexConstraint(7, 200);
    const std::vector<int> indicesInCycle{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    std::vector<int> c = g.findConstrainedVerticesInCycle(indicesInCycle);
    EXPECT_EQ(3, c.size());
}

TEST(Cycle, FindingConstraintsInTriangleWithOneConstraintWorks) {
    auto g = generateTriangleGraph();
    g.addVertexConstraint(1, 100);
    const std::vector<int> indicesInCycle{1, 2, 3, 1};
    std::vector<int> c = g.findConstrainedVerticesInCycle(indicesInCycle);
    EXPECT_EQ(1, c.size());
}

TEST(Cycle, SplittingACycleIntoTwoPathsWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    const std::vector<int> indicesInCycle{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    const std::vector<int> cutPoints{4, 8};
    const auto paths = g.splitCycle(indicesInCycle, cutPoints);
    EXPECT_EQ(2, paths.size());
    EXPECT_EQ(5, paths[0].size());
    EXPECT_EQ(7, paths[1].size());
    const std::vector<std::vector<int> > expected{{4, 5, 6, 7, 8}, {8, 9, 10, 1, 2, 3, 4}};
    EXPECT_TRUE(paths == expected);
}

TEST(Cycle, SplittingACycleIntoMultiplePathsWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    const std::vector<int> indicesInCycle{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1};
    const std::vector<int> cutPoints{2, 4, 8};
    const auto paths = g.splitCycle(indicesInCycle, cutPoints);
    EXPECT_EQ(3, paths.size());
    EXPECT_EQ(3, paths[0].size());
    EXPECT_EQ(5, paths[1].size());
    EXPECT_EQ(5, paths[1].size());
    const std::vector<std::vector<int> > expected{{2, 3, 4}, {4, 5, 6, 7, 8}, 
        {8, 9, 10, 1, 2}};
    EXPECT_TRUE(paths == expected);
}

TEST(Misc, CountingNumEdgesWorks) {
    auto g = generateSimpleLoopGraphWith10Vertices();
    EXPECT_EQ(10, g.numEdges());
}

TEST(Pathfinding, GettingPathsFromATreeWihtNoConstraintsWorks) {
    auto g = generateSimpleTreeGraph();
    const auto& path = g.findPath();
    const std::vector<int> expected{1, 2, 3};
    EXPECT_TRUE(expected == path);
    auto tempG = generateEmptyGraph();
    for(size_t i = 0; i < path.size()-1; i++) {
        const int v1 = path[i], v2 = path[i+1];
        g.moveEdgeToAnotherGraph(tempG, v1, v2);
    }
    const auto& path2 = g.findPath();
    const std::vector<int> expected2{1, 4, 5, 6};
    EXPECT_TRUE(expected2 == path2);
}

TEST(Pathfinding, GettingPathsFromATreeReturnsMostConstrainedPathWith2First) {
    auto g = generateSimpleTreeGraph();
    g.addVertexConstraint(3, 1);
    g.addVertexConstraint(7, 1);
    const auto& path = g.findPath();
    EXPECT_EQ(3, path.front());
    EXPECT_EQ(7, path.back());
}

TEST(Pathfinding, GettingPathFromTreeReturnsMostContrainedPathWith1ConstraintFirst) {
    auto g = generateSimpleTreeGraph();
    g.addVertexConstraint(8, 1);
    const auto& path= g.findPath();
    EXPECT_EQ(8, path.front());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}