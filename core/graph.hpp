#ifndef DS_GRAPH_HPP
#define DS_GRAPH_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include "queue.hpp" // reused for BFS traversal (built in Week 1)

namespace ds {

// A weighted, directed-by-default graph stored as an adjacency list.
// No STL containers used — vertices live in a raw, resizable array;
// each vertex's edges live in a small manually-managed linked list.
class Graph {
private:
    struct Edge {
        size_t target;
        double weight;
        Edge* next;
        Edge(size_t t, double w, Edge* n) : target(t), weight(w), next(n) {}
    };

    struct Vertex {
        std::string label;
        Edge* edgeHead;
        bool active;
        Vertex() : label(), edgeHead(nullptr), active(false) {}
    };

    Vertex* vertices;
    size_t capacity;
    size_t count; // number of vertex slots used so far (includes removed ones)

    void resize(size_t newCapacity) {
        Vertex* newVertices = new Vertex[newCapacity];
        for (size_t i = 0; i < count; ++i) {
            newVertices[i] = vertices[i]; // transfers ownership of each edge list
        }
        delete[] vertices;
        vertices = newVertices;
        capacity = newCapacity;
    }

    void freeEdges(Edge* head) {
        while (head != nullptr) {
            Edge* next = head->next;
            delete head;
            head = next;
        }
    }

    // Index must exist (may or may not still be active).
    void checkBounds(size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Vertex index out of range");
        }
    }

    // Index must exist AND still be an active (non-removed) vertex.
    void checkActive(size_t index) const {
        checkBounds(index);
        if (!vertices[index].active) {
            throw std::invalid_argument("Vertex has been removed");
        }
    }

public:
    explicit Graph(size_t initialCapacity = 8)
        : capacity(initialCapacity == 0 ? 1 : initialCapacity), count(0) {
        vertices = new Vertex[capacity];
    }

    ~Graph() {
        for (size_t i = 0; i < count; ++i) {
            freeEdges(vertices[i].edgeHead);
        }
        delete[] vertices;
    }

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    // Adds a new vertex (e.g. a word) and returns its index.
    size_t addVertex(const std::string& label) {
        if (count == capacity) {
            resize(capacity * 2);
        }
        vertices[count].label = label;
        vertices[count].edgeHead = nullptr;
        vertices[count].active = true;
        return count++;
    }

    // Adds a directed, weighted edge from -> to.
    void addEdge(size_t from, size_t to, double weight = 1.0) {
        checkActive(from);
        checkActive(to);
        vertices[from].edgeHead = new Edge(to, weight, vertices[from].edgeHead);
    }

    // Convenience: adds the edge in both directions (e.g. "shares a kanji" is mutual).
    void addUndirectedEdge(size_t a, size_t b, double weight = 1.0) {
        addEdge(a, b, weight);
        addEdge(b, a, weight);
    }

    bool hasEdge(size_t from, size_t to) const {
        checkBounds(from);
        Edge* current = vertices[from].edgeHead;
        while (current != nullptr) {
            if (current->target == to) return true;
            current = current->next;
        }
        return false;
    }

    void removeEdge(size_t from, size_t to) {
        checkBounds(from);
        Edge** current = &vertices[from].edgeHead;
        while (*current != nullptr) {
            if ((*current)->target == to) {
                Edge* toDelete = *current;
                *current = (*current)->next;
                delete toDelete;
                return;
            }
            current = &(*current)->next;
        }
    }

    // Removes a vertex: clears its own edges, and removes any edges
    // other vertices had pointing INTO it, so nothing dangles.
    void removeVertex(size_t index) {
        checkActive(index);
        for (size_t i = 0; i < count; ++i) {
            if (i == index) continue;
            removeEdge(i, index);
        }
        freeEdges(vertices[index].edgeHead);
        vertices[index].edgeHead = nullptr;
        vertices[index].active = false;
    }

    bool isActive(size_t index) const {
        checkBounds(index);
        return vertices[index].active;
    }

    size_t vertexCount() const {
        size_t total = 0;
        for (size_t i = 0; i < count; ++i) {
            if (vertices[i].active) ++total;
        }
        return total;
    }

    const std::string& label(size_t index) const {
        checkBounds(index);
        return vertices[index].label;
    }

    // Calls fn(neighborIndex, edgeWeight) for every neighbor of `index`.
    // A callback is used instead of returning a container, since
    // std::vector / std::list etc. are off-limits.
    template <typename Func>
    void forEachNeighbor(size_t index, Func fn) const {
        checkBounds(index);
        Edge* current = vertices[index].edgeHead;
        while (current != nullptr) {
            fn(current->target, current->weight);
            current = current->next;
        }
    }

    // Breadth-first traversal from `start`. Calls visit(vertexIndex)
    // once for every reachable vertex, in BFS (level-by-level) order.
    template <typename Func>
    void bfs(size_t start, Func visit) const {
        checkActive(start);

        bool* visited = new bool[capacity];
        for (size_t i = 0; i < capacity; ++i) visited[i] = false;

        ds::Queue<size_t> frontier; // reusing Week 1's Queue
        visited[start] = true;
        frontier.enqueue(start);

        while (!frontier.isEmpty()) {
            size_t current = frontier.dequeue();
            visit(current);

            Edge* edge = vertices[current].edgeHead;
            while (edge != nullptr) {
                if (!visited[edge->target] && vertices[edge->target].active) {
                    visited[edge->target] = true;
                    frontier.enqueue(edge->target);
                }
                edge = edge->next;
            }
        }

        delete[] visited;
    }
};

} // namespace ds

#endif // DS_GRAPH_HPP
