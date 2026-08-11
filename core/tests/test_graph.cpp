#include <iostream>
#include "../graph.hpp"

int failures = 0;

#define CHECK(condition, description) \
    do { \
        if (condition) { \
            std::cout << "[PASS] " << description << std::endl; \
        } else { \
            std::cout << "[FAIL] " << description << std::endl; \
            ++failures; \
        } \
    } while (0)

void test_add_vertices_and_directed_edge() {
    ds::Graph g;
    size_t neko = g.addVertex("neko");
    size_t inu = g.addVertex("inu");

    CHECK(g.vertexCount() == 2, "vertexCount is 2 after adding two vertices");
    g.addEdge(neko, inu, 2.5);
    CHECK(g.hasEdge(neko, inu), "directed edge exists from neko to inu");
    CHECK(!g.hasEdge(inu, neko), "directed edge does not exist in reverse direction");
}

void test_undirected_edge_both_directions() {
    ds::Graph g;
    size_t a = g.addVertex("a");
    size_t b = g.addVertex("b");
    g.addUndirectedEdge(a, b, 1.0);

    CHECK(g.hasEdge(a, b), "undirected edge exists a -> b");
    CHECK(g.hasEdge(b, a), "undirected edge exists b -> a");
}

void test_neighbor_lookup_and_weights() {
    ds::Graph g;
    size_t a = g.addVertex("a");
    size_t b = g.addVertex("b");
    size_t c = g.addVertex("c");
    g.addEdge(a, b, 3.0);
    g.addEdge(a, c, 5.0);

    double totalWeight = 0.0;
    size_t neighborCount = 0;
    g.forEachNeighbor(a, [&](size_t target, double weight) {
        totalWeight += weight;
        ++neighborCount;
    });

    CHECK(neighborCount == 2, "vertex a reports exactly 2 neighbors");
    CHECK(totalWeight == 8.0, "sum of edge weights from a is 8.0 (3.0 + 5.0)");
}

void test_remove_edge() {
    ds::Graph g;
    size_t a = g.addVertex("a");
    size_t b = g.addVertex("b");
    g.addEdge(a, b, 1.0);

    CHECK(g.hasEdge(a, b), "edge exists before removal");
    g.removeEdge(a, b);
    CHECK(!g.hasEdge(a, b), "edge no longer exists after removeEdge");
}

void test_remove_vertex_cleans_up_incoming_edges() {
    ds::Graph g;
    size_t a = g.addVertex("a");
    size_t b = g.addVertex("b");
    size_t c = g.addVertex("c");
    g.addEdge(a, b, 1.0);
    g.addEdge(c, b, 1.0);

    g.removeVertex(b);

    CHECK(!g.isActive(b), "removed vertex is no longer active");
    CHECK(!g.hasEdge(a, b), "edge a -> b was cleaned up after b was removed");
    CHECK(!g.hasEdge(c, b), "edge c -> b was cleaned up after b was removed");
    CHECK(g.vertexCount() == 2, "vertexCount correctly excludes the removed vertex");
}

void test_bfs_visits_directed_chain_in_order() {
    ds::Graph g;
    size_t a = g.addVertex("A");
    size_t b = g.addVertex("B");
    size_t c = g.addVertex("C");
    size_t d = g.addVertex("D");
    g.addEdge(a, b);
    g.addEdge(b, c);
    g.addEdge(c, d);

    size_t order[4];
    size_t idx = 0;
    g.bfs(a, [&](size_t v) { order[idx++] = v; });

    CHECK(idx == 4, "BFS visits all 4 vertices in the chain");
    CHECK(order[0] == a && order[1] == b && order[2] == c && order[3] == d,
          "BFS visits the chain in correct order: A, B, C, D");
}

void test_bfs_reaches_every_vertex_in_a_star() {
    ds::Graph g;
    size_t center = g.addVertex("center");
    size_t n1 = g.addVertex("n1");
    size_t n2 = g.addVertex("n2");
    size_t n3 = g.addVertex("n3");
    g.addUndirectedEdge(center, n1);
    g.addUndirectedEdge(center, n2);
    g.addUndirectedEdge(center, n3);

    bool visited[4] = {false, false, false, false};
    size_t visitCount = 0;
    size_t firstVisited = 999;
    g.bfs(center, [&](size_t v) {
        if (visitCount == 0) firstVisited = v;
        visited[v] = true;
        ++visitCount;
    });

    CHECK(visitCount == 4, "BFS visits the center plus all 3 neighbors");
    CHECK(firstVisited == center, "BFS starts at the given start vertex");
    CHECK(visited[center] && visited[n1] && visited[n2] && visited[n3],
          "BFS reaches every vertex in the star graph");
}

void test_bfs_does_not_reach_disconnected_vertex() {
    ds::Graph g;
    size_t a = g.addVertex("A");
    size_t b = g.addVertex("B");
    size_t isolated = g.addVertex("isolated");
    g.addEdge(a, b);

    bool reachedIsolated = false;
    size_t count = 0;
    g.bfs(a, [&](size_t v) {
        ++count;
        if (v == isolated) reachedIsolated = true;
    });

    CHECK(count == 2, "BFS only visits reachable vertices (A and B)");
    CHECK(!reachedIsolated, "BFS does not visit a disconnected vertex");
}

int main() {
    test_add_vertices_and_directed_edge();
    test_undirected_edge_both_directions();
    test_neighbor_lookup_and_weights();
    test_remove_edge();
    test_remove_vertex_cleans_up_incoming_edges();
    test_bfs_visits_directed_chain_in_order();
    test_bfs_reaches_every_vertex_in_a_star();
    test_bfs_does_not_reach_disconnected_vertex();

    std::cout << std::endl;
    if (failures == 0) {
        std::cout << "All Graph tests passed." << std::endl;
    } else {
        std::cout << failures << " Graph test(s) failed." << std::endl;
    }
    return failures == 0 ? 0 : 1;
}
