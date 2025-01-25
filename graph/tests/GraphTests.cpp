#include "../src/containers/UndirectedGraph.h"
#include "../src/containers/DirectedGraph.h"
#include <gtest/gtest.h>

TEST(UndirectedGraphTest, AddVertex) {
    UndirectedGraph<int> graph;
    graph.addVertex(1);
    EXPECT_NO_THROW(graph.addVertex(2));
    EXPECT_THROW(graph.addVertex(1), std::invalid_argument);
}

TEST(UndirectedGraphTest, AddEdge) {
    UndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);

    EXPECT_NO_THROW(graph.addEdge(1, 2, 5.0));
    EXPECT_THROW(graph.addEdge(1, 3, 2.0), std::invalid_argument);
}

TEST(UndirectedGraphTest, RemoveVertex) {
    UndirectedGraph<std::string> graph;
    graph.addVertex("A");
    graph.addVertex("B");
    graph.addEdge("A", "B", 3.5);

    EXPECT_NO_THROW(graph.removeVertex("B"));
    EXPECT_THROW(graph.removeVertex("B"), std::invalid_argument);
}

TEST(UndirectedGraphTest, RemoveEdge) {
    UndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addEdge(1, 2, 4.5);

    EXPECT_NO_THROW(graph.removeEdge(1, 2));
    EXPECT_THROW(graph.removeEdge(1, 3), std::invalid_argument);
}

TEST(UndirectedGraphTest, BFS) {
    UndirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);

    DynamicArray<int> bfsResult = graph.BFS(1);
    DynamicArray<int> expected (new int[4]{1, 2, 3, 4}, 4, 4);

    EXPECT_EQ(bfsResult.getLength(), expected.getLength());
    for (int i = 0; i < bfsResult.getLength(); ++i) {
        EXPECT_EQ(bfsResult[i], expected[i]);
    }
}

TEST(UndirectedGraphTest, DFS) {
    UndirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);

    DynamicArray<int> dfsResult = graph.DFS(1);
    DynamicArray<int> expected (new int[4]{1, 2, 4, 3}, 4, 4);
    EXPECT_EQ(dfsResult.getLength(), expected.getLength());
    for (int i = 0; i < dfsResult.getLength(); ++i) {
        EXPECT_EQ(dfsResult[i], expected[i]);
    }
}

TEST(UndirectedGraphTest, Dijkstra) {
    UndirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 4);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 4, 6);
    graph.addEdge(3, 4, 3);

    HashTable<int, double> distances = graph.Dijkstra(1);
    EXPECT_EQ(distances.get(1), 0);
    EXPECT_EQ(distances.get(2), 1);
    EXPECT_EQ(distances.get(3), 3);
    EXPECT_EQ(distances.get(4), 6);
}

TEST(UndirectedGraphTest, ErsovColoringTest) {
    UndirectedGraph<std::string> graph;
    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");
    graph.addVertex("D");
    graph.addVertex("E");
    graph.addEdge("A", "B", 1.0);
    graph.addEdge("A", "C", 1.0);
    graph.addEdge("B", "D", 1.0);
    graph.addEdge("C", "D", 1.0);
    graph.addEdge("D", "E", 1.0);
    auto result = graph.ErshovColoring();
    std::map<std::string, int> expectedColors = {
            {"A", 0},
            {"B", 1},
            {"C", 1},
            {"D", 0},
            {"E", 1},
    };

    for (int i = 0; i < result.getLength(); ++i) {
        const auto& pair = result[i];
        const std::string& vertex = pair.first;
        int color = pair.second;
        ASSERT_EQ(expectedColors[vertex], color);
    }
}

TEST(DirectedGraphTest, AddVertex) {
    DirectedGraph<int> graph;
    graph.addVertex(1);
    EXPECT_NO_THROW(graph.addVertex(2));
    EXPECT_THROW(graph.addVertex(1), std::invalid_argument);
}

// Тест на добавление ребра
TEST(DirectedGraphTest, AddEdge) {
    DirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);

    EXPECT_NO_THROW(graph.addEdge(1, 2, 5.0));
    EXPECT_THROW(graph.addEdge(1, 3, 2.0), std::invalid_argument);
    EXPECT_THROW(graph.addEdge(3, 1, 2.0), std::invalid_argument);
}

TEST(DirectedGraphTest, RemoveVertex) {
    DirectedGraph<std::string> graph;
    graph.addVertex("A");
    graph.addVertex("B");
    graph.addEdge("A", "B", 3.5);

    EXPECT_NO_THROW(graph.removeVertex("B"));
    EXPECT_THROW(graph.removeVertex("B"), std::invalid_argument);
}

TEST(DirectedGraphTest, RemoveEdge) {
    DirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addEdge(1, 2, 4.5);

    EXPECT_NO_THROW(graph.removeEdge(1, 2));
    EXPECT_THROW(graph.removeEdge(1, 3), std::invalid_argument);
    std::cout << graph.getVertices()[0] << std::endl;
}

TEST(DirectedGraphTest, DFS) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);

    DynamicArray<int> dfsResult = graph.DFS(1);
    DynamicArray<int> expected (new int[4]{1, 2, 4, 3}, 4, 4);
    EXPECT_EQ(dfsResult.getLength(), expected.getLength());
    for (int i = 0; i < dfsResult.getLength(); ++i) {
        EXPECT_EQ(dfsResult[i], expected[i]);
    }
}

TEST(DirectedGraphTest, BFS) {
    DirectedGraph<int> graph;
    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 4, 1);

    DynamicArray<int> bfsResult = graph.BFS(1);

    DynamicArray<int> expected (new int[4]{1, 2, 3, 4}, 4, 4);

    EXPECT_EQ(bfsResult.getLength(), expected.getLength());
    for (int i = 0; i < bfsResult.getLength(); ++i) {
        EXPECT_EQ(bfsResult[i], expected[i]);
    }
}


TEST(DirectedGraphTest, Dijkstra) {
    DirectedGraph<int> graph;

    graph.addVertex(1);
    graph.addVertex(2);
    graph.addVertex(3);
    graph.addVertex(4);

    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 4);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 4, 6);
    graph.addEdge(3, 4, 3);

    HashTable<int, double> distances = graph.Dijkstra(1);
    EXPECT_EQ(distances.get(1), 0);
    EXPECT_EQ(distances.get(2), 1);
    EXPECT_EQ(distances.get(3), 3);
    EXPECT_EQ(distances.get(4), 6);
}



